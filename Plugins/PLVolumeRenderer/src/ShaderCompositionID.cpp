/*********************************************************\
 *  File: ShaderCompositionID.cpp                        *
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
#include <PLCore/Base/Class.h>
#include <PLCore/Tools/ChecksumCRC32.h>
#include <PLRenderer/Renderer/ShaderLanguage.h>
#include "PLVolumeRenderer/ShaderCompositionID.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderCompositionID::ShaderCompositionID() :
	m_nChecksumCRC32(0)
{
	MemoryManager::Set(&m_sData, 0, sizeof(SData));
}

/**
*  @brief
*    Copy constructor
*/
ShaderCompositionID::ShaderCompositionID(const ShaderCompositionID &cSource) :
	m_nChecksumCRC32(cSource.m_nChecksumCRC32)
{
	MemoryManager::Copy(&m_sData, &cSource.m_sData, sizeof(SData));
}

/**
*  @brief
*    Copy operator
*/
ShaderCompositionID &ShaderCompositionID::operator =(const ShaderCompositionID &cSource)
{
	// Copy data
	m_nChecksumCRC32 = cSource.m_nChecksumCRC32;
	MemoryManager::Copy(&m_sData, &cSource.m_sData, sizeof(SData));

	// Return a pointer to this instance
	return *this;
}

/**
*  @brief
*    Check for equality
*/
bool ShaderCompositionID::operator ==(const ShaderCompositionID &cOther) const
{
	// First fast test: Check for CRC32 checksum match
	// Second: Check the shader composition data the ID consists of (the CRC32 checksum is not unique)
	return ((m_nChecksumCRC32 == cOther.m_nChecksumCRC32) && !MemoryManager::Compare(&m_sData, &cOther.m_sData, sizeof(SData)));
}

/**
*  @brief
*    Returns a human readable representation of this shader composition ID
*/
String ShaderCompositionID::ToString() const
{
	// Is this shader composition ID valid?
	if (IsValid()) {
		String sString = String("CRC32Checksum=") + m_nChecksumCRC32 + ' ';
		sString += "ShaderLanguage=\"" + m_sData.pShaderLanguage->GetShaderLanguage() + "\" ";

		// 1.0 - Ray Setup
		sString += "1.0_RaySetup=\"" + m_sData.pShaderFunctionRaySetupClass->GetClassName() + "\" ";

		// 1.1 - Clip Ray
		sString += "1.2_ClipRay=\"" + m_sData.pShaderFunctionClipRayClass->GetClassName() + "\" (NumOfClipPlanes=" + m_sData.nNumOfClipPlanes + " NumOfDepthTextures=" + m_sData.nNumOfDepthTextures + ") ";

		// 1.2 - Jitter Position
		sString += "1.1_JitterPosition=\"" + m_sData.pShaderFunctionJitterPositionClass->GetClassName() + "\" ";

		// 2.0 - Ray Traversal
		sString += "2.0_RayTraversal=\"" + m_sData.pShaderFunctionRayTraversalClass->GetClassName() + "\" ";

		// 2.1 - Clip Position
		sString += "2.1_ClipPosition=\"" + m_sData.pShaderFunctionClipPositionClass->GetClassName() + "\" ";

		// 2.2 - Reconstruction
		sString += "2.2_Reconstruction=\"" + m_sData.pShaderFunctionReconstructionClass->GetClassName() + "\" ";

		// 2.2 - Fetch Scalar
		sString += "2.2_FetchScalar=\"" + m_sData.pShaderFunctionFetchScalarClass->GetClassName() + "\" ";

		// 2.3 - Shading
		sString += "2.3_Shading=\"" + m_sData.pShaderFunctionShadingClass->GetClassName() + "\" ";

		// 2.4 - Classification
		sString += "2.4_Classification=\"" + m_sData.pShaderFunctionClassificationClass->GetClassName() + "\" ";

		// 2.5 - Gradient
		sString += "2.5_Gradient=\"" + m_sData.pShaderFunctionGradientClass->GetClassName() + '\" ';

		// 2.5 - Gradient Input
		sString += "2.5_GradientInput=\"" + m_sData.pShaderFunctionGradientInputClass->GetClassName() + '\" ';

		// 2.6 - Illumination
		sString += "2.6_Illumination=\"" + m_sData.pShaderFunctionIlluminationClass->GetClassName() + '\"';

		// Done
		return sString;
	}

	// Error!
	return "";
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Calculates the CRC32 checksum
*/
void ShaderCompositionID::CalculateChecksum()
{
	m_nChecksumCRC32 = ChecksumCRC32().GetChecksum(reinterpret_cast<uint8*>(&m_sData), sizeof(SData));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
