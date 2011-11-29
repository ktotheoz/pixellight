/*********************************************************\
 *  File: ShaderLanguage.cpp                             *
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
#include "PLRenderer/Renderer/Program.h"
#include "PLRenderer/Renderer/VertexShader.h"
#include "PLRenderer/Renderer/FragmentShader.h"
#include "PLRenderer/Renderer/ShaderLanguage.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderLanguage)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a vertex shader and sets the shader source code
*/
VertexShader *ShaderLanguage::CreateVertexShader(const String &sSourceCode, const String &sProfile, const String &sEntry)
{
	// Create the vertex shader instance
	VertexShader *pVertexShader = CreateVertexShader();

	// Set the vertex shader source code
	if (pVertexShader)
		pVertexShader->SetSourceCode(sSourceCode, sProfile, sEntry);

	// Return the created vertex shader instance
	return pVertexShader;
}

/**
*  @brief
*    Creates a geometry shader and sets the shader source code
*/
GeometryShader *ShaderLanguage::CreateGeometryShader(const String &sSourceCode, GeometryShader::EInputPrimitiveType nInputPrimitiveType, GeometryShader::EOutputPrimitiveType nOutputPrimitiveType, uint32 nNumOfOutputVertices, const String &sProfile, const String &sEntry)
{
	// Create the geometry shader instance
	GeometryShader *pGeometryShader = CreateGeometryShader();

	// Set the geometry shader source code
	if (pGeometryShader)
		pGeometryShader->SetSourceCode(sSourceCode, nInputPrimitiveType, nOutputPrimitiveType, nNumOfOutputVertices, sProfile, sEntry);

	// Return the created geometry shader instance
	return pGeometryShader;
}

/**
*  @brief
*    Creates a fragment shader and sets the shader source code
*/
FragmentShader *ShaderLanguage::CreateFragmentShader(const String &sSourceCode, const String &sProfile, const String &sEntry)
{
	// Create the fragment shader instance
	FragmentShader *pFragmentShader = CreateFragmentShader();

	// Set the fragment shader source code
	if (pFragmentShader)
		pFragmentShader->SetSourceCode(sSourceCode, sProfile, sEntry);

	// Return the created fragment shader instance
	return pFragmentShader;
}

/**
*  @brief
*    Creates a program and assigns a vertex and fragment shader to it
*/
Program *ShaderLanguage::CreateProgram(VertexShader *pVertexShader, FragmentShader *pFragmentShader)
{
	// Create a program instance
	Program *pProgram = CreateProgram();
	if (pProgram) {
		// Assign the given vertex and fragment shaders to the program
		pProgram->SetVertexShader(pVertexShader);
		pProgram->SetFragmentShader(pFragmentShader);
	}

	// Return the created program instance
	return pProgram;
}

/**
*  @brief
*    Creates a program and assigns a vertex, geometry and fragement shader to it
*/
Program *ShaderLanguage::CreateProgram(VertexShader *pVertexShader, GeometryShader *pGeometryShader, FragmentShader *pFragmentShader)
{
	// Create a program instance
	Program *pProgram = CreateProgram();
	if (pProgram) {
		// Assign the given vertex, geometry and fragment shaders to the program
		pProgram->SetVertexShader(pVertexShader);
		pProgram->SetGeometryShader(pGeometryShader);
		pProgram->SetFragmentShader(pFragmentShader);
	}

	// Return the created program instance
	return pProgram;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ShaderLanguage::ShaderLanguage()
{
}

/**
*  @brief
*    Destructor
*/
ShaderLanguage::~ShaderLanguage()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
ShaderLanguage::ShaderLanguage(const ShaderLanguage &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
ShaderLanguage &ShaderLanguage::operator =(const ShaderLanguage &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
