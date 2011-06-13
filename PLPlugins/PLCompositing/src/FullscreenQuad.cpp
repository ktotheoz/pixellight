/*********************************************************\
 *  File: FullscreenQuad.cpp                             *
 *
 *  Copyright (C) 2002-2011 The PixelLight Team (http://www.pixellight.org/)
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
#include <PLMath/Vector4.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLCompositing/FullscreenQuad.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLMath;
using namespace PLRenderer;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FullscreenQuad::FullscreenQuad(Renderer &cRenderer) :
	m_pRenderer(&cRenderer),
	m_pVertexBuffer(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
FullscreenQuad::~FullscreenQuad()
{
	// Destroy the vertex buffer
	if (m_pVertexBuffer)
		delete m_pVertexBuffer;
}

/**
*  @brief
*    Returns the vertex buffer of this fullscreen quad
*/
VertexBuffer *FullscreenQuad::GetVertexBuffer()
{
	// Initialize vertex buffer
	if (!m_pVertexBuffer) {
		// Create the vertex buffer
		m_pVertexBuffer = m_pRenderer->CreateVertexBuffer();

		// Add vertex position attribute to the vertex buffer, zw stores the texture coordinate
		m_pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float4);

		// Allocate
		m_pVertexBuffer->Allocate(4);

		// Fill
		if (m_pVertexBuffer->Lock(Lock::WriteOnly)) {
		// Vertex 0 - lower/left corner
			// Position
			float *pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(0, VertexBuffer::Position));
			pfVertex[Vector4::X] = -1.0f;
			pfVertex[Vector4::Y] = -1.0f;
			// Texture coordinate
			pfVertex[Vector4::Z] =  0.0f;
			pfVertex[Vector4::W] =  0.0f;

		// Vertex 1 - lower/right corner
			// Position
			pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(1, VertexBuffer::Position));
			pfVertex[Vector4::X] =  1.0f;
			pfVertex[Vector4::Y] = -1.0f;
			// Texture coordinate
			pfVertex[Vector4::Z] =  1.0f;
			pfVertex[Vector4::W] =  0.0f;

		// Vertex 2 - upper/left corner
			// Position
			pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(2, VertexBuffer::Position));
			pfVertex[Vector4::X] = -1.0f;
			pfVertex[Vector4::Y] =  1.0f;
			// Texture coordinate
			pfVertex[Vector4::Z] =  0.0f;
			pfVertex[Vector4::W] =  1.0f;

		// Vertex 3 - upper/right corner
			// Position
			pfVertex = static_cast<float*>(m_pVertexBuffer->GetData(3, VertexBuffer::Position));
			pfVertex[Vector4::X] = 1.0f;
			pfVertex[Vector4::Y] = 1.0f;
			// Texture coordinate
			pfVertex[Vector4::Z] = 1.0f;
			pfVertex[Vector4::W] = 1.0f;

			// Unlock the vertex buffer
			m_pVertexBuffer->Unlock();
		}
	}

	// Return the vertex buffer
	return m_pVertexBuffer;
}

/**
*  @brief
*    Draws the fullscreen quad
*/
void FullscreenQuad::Draw(bool bSetupRenderer)
{
	// Setup renderer?
	uint32 nFixedFillModeBackup = 0;
	if (bSetupRenderer) {
		nFixedFillModeBackup = m_pRenderer->GetRenderState(RenderState::FixedFillMode);
		m_pRenderer->SetRenderState(RenderState::FixedFillMode, Fill::Solid);
		m_pRenderer->SetRenderState(RenderState::CullMode,		Cull::None);
		m_pRenderer->SetRenderState(RenderState::ZEnable,		false);
		m_pRenderer->SetRenderState(RenderState::ZWriteEnable,	false);
	}

	// Draw the fullscreen quad
	m_pRenderer->DrawPrimitives(Primitive::TriangleStrip, 0, 4);

	// Restore fixed fill mode render state?
	if (bSetupRenderer)
		m_pRenderer->SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing