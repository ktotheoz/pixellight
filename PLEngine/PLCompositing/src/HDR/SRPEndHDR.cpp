/*********************************************************\
 *  File: SRPEndHDR.cpp                                  *
 *
 *  Copyright (C) 2002-2010 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <float.h>
#include <PLGeneral/Tools/Timing.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLScene/Compositing/FullscreenQuad.h>
#include <PLScene/Compositing/General/SRPBegin.h>
#include "PLCompositing/HDR/HDRAverageLuminance.h"
#include "PLCompositing/HDR/HDRLightAdaptation.h"
#include "PLCompositing/HDR/HDRBloom.h"
#include "PLCompositing/HDR/SRPEndHDR.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPEndHDR)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPEndHDR::SRPEndHDR() :
	ShaderLanguage(this),
	LuminanceConvert(this),
	Key(this),
	WhiteLevel(this),
	AverageLuminance(this),
	Tau(this),
	BloomBrightThreshold(this),
	BloomFactor(this),
	BloomBlurPasses(this),
	BloomDownscale(this),
	Gamma(this),
	Flags(this),
	m_pFullscreenQuad(NULL),
	m_pHDRAverageLuminance(NULL),
	m_pHDRLightAdaptation(NULL),
	m_pHDRBloom(NULL),
	m_pProgramGenerator(NULL)
{
}

/**
*  @brief
*    Destructor
*/
SRPEndHDR::~SRPEndHDR()
{
	// Destroy the HDR logarithmic average luminance calculation component
	if (m_pHDRAverageLuminance)
		delete m_pHDRAverageLuminance;

	// Destroy the HDR light adaptation calculation component
	if (m_pHDRLightAdaptation)
		delete m_pHDRLightAdaptation;

	// Destroy the HDR bloom calculation component
	if (m_pHDRBloom)
		delete m_pHDRBloom;

	// Destroy the fullscreen quad
	if (m_pFullscreenQuad)
		delete m_pFullscreenQuad;

	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneRendererPass functions ]
//[-------------------------------------------------------]
void SRPEndHDR::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the "PLScene::SRPBegin" instance
	SRPBegin *pSRPBegin = (SRPBegin*)GetFirstInstanceOfSceneRendererPassClass("PLScene::SRPBegin");
	if (pSRPBegin) {
		// Get the back render target of SRPBegin, this holds the current content
		SurfaceTextureBuffer *pSurfaceTextureBuffer = pSRPBegin->GetBackRenderTarget();
		if (pSurfaceTextureBuffer) {
			// Create the fullscreen quad instance if required
			if (!m_pFullscreenQuad)
				m_pFullscreenQuad = new FullscreenQuad(cRenderer);

			// Get the vertex buffer of the fullscreen quad
			VertexBuffer *pVertexBuffer = m_pFullscreenQuad->GetVertexBuffer();
			if (pVertexBuffer) {
				// Get the shader language to use
				String sShaderLanguage = ShaderLanguage;
				if (!sShaderLanguage.GetLength())
					sShaderLanguage = cRenderer.GetDefaultShaderLanguage();

				// Create the program generator if there's currently no instance of it
				if (!m_pProgramGenerator || m_pProgramGenerator->GetShaderLanguage() != sShaderLanguage) {
					// If there's an previous instance of the program generator, destroy it first
					if (m_pProgramGenerator) {
						delete m_pProgramGenerator;
						m_pProgramGenerator = NULL;
					}

					// Choose the shader source codes depending on the requested shader language
					String sEndHDR_VS;
					String sEndHDR_FS;
					if (sShaderLanguage == "GLSL") {
						#include "SRPEndHDR_GLSL.h"
						sEndHDR_VS = sEndHDR_GLSL_VS;
						sEndHDR_FS = sEndHDR_GLSL_FS;
					} else if (sShaderLanguage == "Cg") {
						#include "SRPEndHDR_Cg.h"
						sEndHDR_VS = sEndHDR_Cg_VS;
						sEndHDR_FS = sEndHDR_Cg_FS;
					}

					// Create the program generator
					if (sEndHDR_VS.GetLength() && sEndHDR_FS.GetLength())
						m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sEndHDR_VS, "glslv", sEndHDR_FS, "glslf", true);
				}

				// If there's no program generator, we don't need to continue
				if (m_pProgramGenerator) {
					// Get the texture we will use as 'source texture' when rendering the fullscreen quad
					TextureBufferRectangle *pHDRTextureBuffer = (TextureBufferRectangle*)pSurfaceTextureBuffer->GetTextureBuffer();

					// Reset all render states to default
					cRenderer.GetRendererContext().GetEffectManager().Use();
					cRenderer.SetColorMask();

					// Get the requested features
					bool bToneMapping				= !(GetFlags() & NoToneMapping);
					bool bAutomaticAverageLuminance = !(GetFlags() & NoAutomaticAverageLuminance);
					bool bLightAdaptation			= !(GetFlags() & NoLightAdaptation);
					bool bBloom						= !(GetFlags() & NoBloom);
					bool bGammaCorrection			= !(GetFlags() & NoGammaCorrection);

					// Calculate average luminance, required for tone mapping with automatically calculated average luminance
					TextureBuffer *pHDRAverageLuminanceTextureBuffer = NULL;
					if (bAutomaticAverageLuminance) {
						if (!m_pHDRAverageLuminance)
							m_pHDRAverageLuminance = new HDRAverageLuminance(cRenderer);
						m_pHDRAverageLuminance->CalculateAverageLuminance((TextureBufferRectangle&)*pSurfaceTextureBuffer->GetTextureBuffer(), LuminanceConvert.Get());

						// Light adaptation
						if (bLightAdaptation && m_pHDRAverageLuminance->GetTextureBuffer()) {
							// [TODO] Would be nice if the "previous" data would not be stored in the scene renderer pass instance...
							if (!m_pHDRLightAdaptation)
								m_pHDRLightAdaptation = new HDRLightAdaptation(cRenderer);
							m_pHDRLightAdaptation->CalculateLightAdaptation(*m_pHDRAverageLuminance->GetTextureBuffer(), Timing::GetInstance()->GetTimeDifference()*Tau);
							pHDRAverageLuminanceTextureBuffer = m_pHDRLightAdaptation->GetTextureBuffer();
						} else {
							pHDRAverageLuminanceTextureBuffer = m_pHDRAverageLuminance->GetTextureBuffer();
						}
					}
					if (!pHDRAverageLuminanceTextureBuffer)
						bAutomaticAverageLuminance = false;

					// Calculate bloom
					if (bBloom) {
						if (BloomDownscale < 1.0f)
							BloomDownscale = 1.0f;
						if (!m_pHDRBloom)
							m_pHDRBloom = new HDRBloom(cRenderer);
						m_pHDRBloom->CalculateBloom((TextureBufferRectangle&)*pSurfaceTextureBuffer->GetTextureBuffer(), BloomBrightThreshold, bToneMapping, bAutomaticAverageLuminance,
													 LuminanceConvert.Get(), Key, WhiteLevel, AverageLuminance, pHDRAverageLuminanceTextureBuffer, BloomBlurPasses, BloomDownscale);

						// Show bloom texture (for debugging)
						if ((GetFlags() & ShowBloomTexture)) {
							bToneMapping			   = false;
							bAutomaticAverageLuminance = false;
							bLightAdaptation		   = false;
							bBloom					   = false;
							bGammaCorrection		   = false;
							pHDRTextureBuffer = (TextureBufferRectangle*)m_pHDRBloom->GetTextureBuffer();
						}
					}

					// Set the current render target back to the orignal one?
					if (GetFlags() & WriteToOriginalSurface)
						cRenderer.SetRenderTarget(pSRPBegin->GetOriginalRenderTarget());
					else
						pSRPBegin->SwapRenderTargets();

					// Reset the program flags
					m_cProgramFlags.Reset();

					// Set program flags
					if (bToneMapping) {
						PL_ADD_FS_FLAG(m_cProgramFlags, FS_TONE_MAPPING)
						if (bAutomaticAverageLuminance)
							PL_ADD_FS_FLAG(m_cProgramFlags, FS_AUTOMATIC_AVERAGE_LUMINANCE)
					}
					if (bBloom)
						PL_ADD_FS_FLAG(m_cProgramFlags, FS_BLOOM)
					if (bGammaCorrection)
						PL_ADD_FS_FLAG(m_cProgramFlags, FS_GAMMA_CORRECTION)

					// Get a program instance from the program generator using the given program flags
					ProgramGenerator::GeneratedProgram *pGeneratedProgram = m_pProgramGenerator->GetProgram(m_cProgramFlags);

					// Make our program to the current one
					if (pGeneratedProgram && cRenderer.SetProgram(pGeneratedProgram->pProgram)) {
						// Set pointers to uniforms & attributes of a generated program if they are not set yet
						GeneratedProgramUserData *pGeneratedProgramUserData = (GeneratedProgramUserData*)pGeneratedProgram->pUserData;
						if (!pGeneratedProgramUserData) {
							pGeneratedProgram->pUserData = pGeneratedProgramUserData = new GeneratedProgramUserData;
							Program *pProgram = pGeneratedProgram->pProgram;
							// Vertex shader attributes
							static const String sVertexPosition = "VertexPosition";
							pGeneratedProgramUserData->pVertexPosition			= pProgram->GetAttribute(sVertexPosition);
							// Vertex shader uniforms
							static const String sTextureSize = "TextureSize";
							pGeneratedProgramUserData->pTextureSize				= pProgram->GetUniform(sTextureSize);
							// Fragment shader uniforms
							static const String sLuminanceConvert = "LuminanceConvert";
							pGeneratedProgramUserData->pLuminanceConvert		= pProgram->GetUniform(sLuminanceConvert);
							static const String sKey = "Key";
							pGeneratedProgramUserData->pKey						= pProgram->GetUniform(sKey);
							static const String sWhiteLevel = "WhiteLevel";
							pGeneratedProgramUserData->pWhiteLevel				= pProgram->GetUniform(sWhiteLevel);
							static const String sAverageLuminanceTexture = "AverageLuminanceTexture";
							pGeneratedProgramUserData->pAverageLuminanceTexture	= pProgram->GetUniform(sAverageLuminanceTexture);
							static const String sAverageLuminance = "AverageLuminance";
							pGeneratedProgramUserData->pAverageLuminance		= pProgram->GetUniform(sAverageLuminance);
							static const String sBloomFactor = "BloomFactor";
							pGeneratedProgramUserData->pBloomFactor				= pProgram->GetUniform(sBloomFactor);
							static const String sBloomDownscale = "BloomDownscale";
							pGeneratedProgramUserData->pBloomDownscale			= pProgram->GetUniform(sBloomDownscale);
							static const String sBloomTexture = "BloomTexture";
							pGeneratedProgramUserData->pBloomTexture			= pProgram->GetUniform(sBloomTexture);
							static const String sInvGamma = "InvGamma";
							pGeneratedProgramUserData->pInvGamma				= pProgram->GetUniform(sInvGamma);
							static const String sHDRTexture = "HDRTexture";
							pGeneratedProgramUserData->pHDRTexture				= pProgram->GetUniform(sHDRTexture);
						}

						// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
						if (pGeneratedProgramUserData->pVertexPosition)
							pGeneratedProgramUserData->pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

						// Set texture size
						if (pGeneratedProgramUserData->pTextureSize)
							pGeneratedProgramUserData->pTextureSize->Set(pHDRTextureBuffer->GetSize());

						// Set tone mapping fragment shader parameters
						if (bToneMapping) { 
							// Set the "LuminanceConvert" fragment shader parameter
							if (pGeneratedProgramUserData->pLuminanceConvert)
								pGeneratedProgramUserData->pLuminanceConvert->Set(LuminanceConvert.Get());

							// Set the "Key" fragment shader parameter
							if (pGeneratedProgramUserData->pKey)
								pGeneratedProgramUserData->pKey->Set((Key > 0.0f) ? Key : 0.0f);

							// Set the "WhiteLevel" fragment shader parameter
							if (pGeneratedProgramUserData->pWhiteLevel)
								pGeneratedProgramUserData->pWhiteLevel->Set((WhiteLevel > 0.0f) ? WhiteLevel : 0.0f);

							// Set the "AverageLuminance" fragment shader parameter
							if (bAutomaticAverageLuminance) {
								if (pGeneratedProgramUserData->pAverageLuminanceTexture) {
									const int nTextureUnit = pGeneratedProgramUserData->pAverageLuminanceTexture->Set(pHDRAverageLuminanceTextureBuffer);
									if (nTextureUnit >= 0) {
										cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
									}
								}
							} else {
								if (pGeneratedProgramUserData->pAverageLuminance)
									pGeneratedProgramUserData->pAverageLuminance->Set(AverageLuminance);
							}
						}

						// Set bloom fragment shader parameters
						if (bBloom) {
							// Set the "BloomFactor" fragment shader parameter
							if (pGeneratedProgramUserData->pBloomFactor)
								pGeneratedProgramUserData->pBloomFactor->Set(BloomFactor);

							// Set the "BloomDownscale" fragment shader parameter
							if (pGeneratedProgramUserData->pBloomDownscale)
								pGeneratedProgramUserData->pBloomDownscale->Set(BloomDownscale);

							// Set the "BloomTexture" fragment shader parameter
							if (pGeneratedProgramUserData->pBloomTexture) {
								const int nTextureUnit = pGeneratedProgramUserData->pBloomTexture->Set(m_pHDRBloom->GetTextureBuffer());
								if (nTextureUnit >= 0) {
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
								}
							}
						}

						// Set the "InvGamma" fragment shader parameter
						if (pGeneratedProgramUserData->pInvGamma) { 
							float fInvGamma = Gamma;
							if (fInvGamma > 0.0f)
								fInvGamma = 1.0f/fInvGamma;
							pGeneratedProgramUserData->pInvGamma->Set((fInvGamma > FLT_MIN) ? fInvGamma : FLT_MIN);
						}

						// Set the "HDRTexture" fragment shader parameter
						if (pGeneratedProgramUserData->pHDRTexture) {
							const int nTextureUnit = pGeneratedProgramUserData->pHDRTexture->Set(pHDRTextureBuffer);
							if (nTextureUnit >= 0) {
								cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
								cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
								cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
								cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
								cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
							}
						}

						// Setup renderer
						const uint32 nFixedFillModeBackup = cRenderer.GetRenderState(RenderState::FixedFillMode);
						cRenderer.SetRenderState(RenderState::ScissorTestEnable, false);
						cRenderer.SetRenderState(RenderState::FixedFillMode,	 Fill::Solid);
						cRenderer.SetRenderState(RenderState::CullMode,			 Cull::None);
						cRenderer.SetRenderState(RenderState::ZEnable,			 false);
						cRenderer.SetRenderState(RenderState::ZWriteEnable,		 false);

						// Draw the fullscreen quad
						cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);

						// Restore fixed fill mode render state
						cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);
					}
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
