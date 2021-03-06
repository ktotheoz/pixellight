/*********************************************************\
 *  File: SPK_PLPointRendererFixedFunctions.cpp          *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Tools/Tools.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Texture/TextureHandler.h>
PL_WARNING_PUSH
PL_WARNING_DISABLE(4530) // "warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc"
	// [HACK] There are missing forward declarations within the SPARK headers...
	namespace SPK {
		class Group;
	}
	#include <Core/SPK_Group.h>
PL_WARNING_POP
#include "SPARK_PL/RenderingAPIs/PixelLight/SPK_PLBuffer.h"
#include "SPARK_PL/RenderingAPIs/PixelLight/SPK_PLPointRendererFixedFunctions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
using namespace PLRenderer;
namespace SPARK_PL {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
SPK_PLPointRendererFixedFunctions *SPK_PLPointRendererFixedFunctions::Create(PLRenderer::Renderer &cRenderer, float fSize)
{
	SPK_PLPointRendererFixedFunctions *pSPK_PLPointRendererFixedFunctions = new SPK_PLPointRendererFixedFunctions(cRenderer, fSize);
	registerObject(pSPK_PLPointRendererFixedFunctions);
	return pSPK_PLPointRendererFixedFunctions;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
SPK_PLPointRendererFixedFunctions::SPK_PLPointRendererFixedFunctions(PLRenderer::Renderer &cRenderer, float fSize) : SPK_PLPointRenderer(cRenderer, fSize),
	m_nTextureBlending(FixedFunctions::TextureEnvironment::Modulate)
{
}

/**
*  @brief
*    Destructor of SPK_PLPointRendererFixedFunctions
*/
SPK_PLPointRendererFixedFunctions::~SPK_PLPointRendererFixedFunctions()
{
}

/**
*  @brief
*    Gets the texture blending function of this SPK_PLQuadRendererFixedFunctions
*/
FixedFunctions::TextureEnvironment::Enum SPK_PLPointRendererFixedFunctions::GetTextureBlending() const
{
	return m_nTextureBlending;
}

/**
*  @brief
*    Sets the texture blending function of this SPK_PLQuadRendererFixedFunctions
*/
void SPK_PLPointRendererFixedFunctions::SetTextureBlending(FixedFunctions::TextureEnvironment::Enum nTextureBlending)
{
	m_nTextureBlending = nTextureBlending;
}


//[-------------------------------------------------------]
//[ Public virtual SPK::Renderer functions                ]
//[-------------------------------------------------------]
void SPK_PLPointRendererFixedFunctions::render(const SPK::Group &group)
{
	// Is there a valid m_pSPK_PLBuffer instance?
	if (m_pSPK_PLBuffer && m_pSPK_PLBuffer->GetVertexBuffer()) {
		// Get the fixed functions interface
		FixedFunctions *pFixedFunctions = GetPLRenderer().GetFixedFunctions();
		if (pFixedFunctions) {
			// Setup common render states
			SetupCommonRenderStates(group);

			// Setup texture
			switch(type) {
				case SPK::POINT_SQUARE:
					GetPLRenderer().SetTextureBuffer();
					break;

				case SPK::POINT_SPRITE:
					m_pTextureHandler->Bind();
					pFixedFunctions->SetTextureStageState(0, FixedFunctions::TextureStage::ColorTexEnv, GetTextureBlending());
					break;

				case SPK::POINT_CIRCLE:
					GetPLRenderer().SetTextureBuffer();
					break;
			}

			// Alpha test
			if (isRenderingHintEnabled(SPK::ALPHA_TEST)) {
				pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestEnable,    true);
				pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestFunction,  Compare::GreaterEqual);
				pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestReference, Tools::FloatToUInt32(getAlphaTestThreshold()));
			} else {
				pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestEnable, false);
			}

			// Make the vertex buffer to the current renderer vertex buffer
			pFixedFunctions->SetVertexBuffer(m_pSPK_PLBuffer->GetVertexBuffer());

			// Draw
			GetPLRenderer().DrawPrimitives(Primitive::PointList, 0, static_cast<uint32>(group.getNbParticles()));
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // SPARK_PL
