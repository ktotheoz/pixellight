/*********************************************************\
 *  File: ShaderFunctionRaySetupNull.cpp                 *
 *
 *  Master thesis
 *    "Scalable Realtime Volume Rendering"
 *
 *  At
 *    Fachhochschule W�rzburg-Schweinfurt
 *    Fakult�t Informatik, Wirtschaftsinformatik (FIW)
 *    http://www.fh-wuerzburg.de/
 *
 *  Author
 *    Christian Ofenberg (c.ofenberg@pixellight.org or cofenberg@googlemail.com)
 *    Copyright (C) 2011-2012
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLVolumeRenderer/RaySetup/ShaderFunctionRaySetupNull.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderFunctionRaySetupNull)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionRaySetupNull::ShaderFunctionRaySetupNull()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionRaySetupNull::~ShaderFunctionRaySetupNull()
{
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionRaySetupNull::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "Null_GLSL.h"

		// Return the requested source code
		switch (nSourceCodeType) {
			case VertexShaderBody:
				return sSourceCode_Vertex;

			case FragmentShaderBody:
				return sSourceCode_Fragment;
		}
	} else if (sShaderLanguage == Cg) {
		#include "Null_Cg.h"

		// Return the requested source code
		switch (nSourceCodeType) {
			case VertexShaderBody:
				return sSourceCode_Vertex;

			case FragmentShaderBody:
				return sSourceCode_Fragment;
		}
	}

	// Error!
	return "";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
