/*********************************************************\
 *  File: Classified_Cg.h                                *
 *      Fragment shader source code - Cg
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
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#define STRINGIFY(ME) #ME


static const PLCore::String sSourceCode_Fragment = STRINGIFY(
/**
*  @brief
*    Fetches the gradient input by using a given position inside the volume
*
*  @param[in] Position
*    Current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*
*  @return
*    Gradient input
*/
float GradientInput(float3 Position)
{
	// Call the fetch scalar function
	//     float FetchScalar(float3 Position)
	// Call the classification function
	//     float4 Classification(float Scalar)
	return Classification(FetchScalar(Position)).a;
}

/**
*  @brief
*    Fetches the gradient input by using a given position inside the volume by using a texel offset
*
*  @param[in] Position
*    Current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in] Offset
*    Texel offset
*
*  @return
*    Gradient input
*/
float GradientInputOffset(float3 Position, int3 Offset)
{
	// Call the fetch scalar function by using a texel offset
	//     float FetchScalarOffset(float3 Position, int3 Offset)
	// Call the classification function
	//     float4 Classification(float Scalar)
	return Classification(FetchScalarOffset(Position, Offset)).a;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
