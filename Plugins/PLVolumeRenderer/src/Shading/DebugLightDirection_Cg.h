/*********************************************************\
 *  File: DebugLightDirection_Cg.h                       *
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
// Uniforms
uniform float3 LightDirection;	// Normalized light direction in scene node space

/**
*  @brief
*    Shading
*
*  @param[in] Scalar
*    Current voxel scalar
*  @param[in] Position
*    Current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in] StepPositionDelta
*    Position advance per step along the ray, within the interval [(0, 0, 0) .. (1, 1, 1)]
*
*  @return
*    RGBA result of the shading
*/
float4 Shading(float Scalar, float3 Position, float3 StepPositionDelta)
{
	// Call the classification function
	//     float4 Classification(float Scalar)
	float4 color = Classification(Scalar);

	// Show normalized light direction
	// -> We can't visualize negative values, so only positive values, please
	return float4(abs(LightDirection), color.a);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
