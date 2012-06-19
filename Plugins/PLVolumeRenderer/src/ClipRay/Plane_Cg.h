/*********************************************************\
 *  File: Plane_Cg.h                                     *
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


// Template version
static const PLCore::String sSourceCode_Fragment_Template = STRINGIFY(
// Uniforms
uniform float4 ClipPlane_x_;	// Clip plane in Hesse normal form in scene node space (xyz=N0, w=p)

/**
*  @brief
*    Performs an clipping operation on the given ray
*
*  @param[in, out] RayOrigin
*    Start position of the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in]      RayDirection
*    Normalized ray direction
*  @param[in, out] MaximumTravelLength
*    Maximum travel length along the ray, within the interval [0 .. 1]
*/
void ClipRay_x_(inout float3 RayOrigin, float3 RayDirection, inout float MaximumTravelLength)
{
	// Calculate the ray end position
	float3 rayEnd = RayOrigin + RayDirection*MaximumTravelLength;

	// Calculate the distance from the ray start and end position to the plane
	float rayOriginDistanceFromPlane = dot(ClipPlane_x_.xyz, RayOrigin) + ClipPlane_x_.w;
	float rayEndDistanceFromPlane = dot(ClipPlane_x_.xyz, rayEnd) + ClipPlane_x_.w;

	// Are both positions on the same side of the plane?
	if (rayOriginDistanceFromPlane*rayEndDistanceFromPlane > 0.0) {
		// Both positions are on the same side of the plane, are we completly clipped?
		if (rayOriginDistanceFromPlane < 0.0)
			MaximumTravelLength = -1.0;
	} else {
		// We intersect the plane, do we need to update the ray origin or the ray end?
		if (rayOriginDistanceFromPlane < 0.0) {
			// Update the ray origin
			RayOrigin -= RayDirection*(rayOriginDistanceFromPlane/dot(ClipPlane_x_.xyz, RayDirection));
		} else {
			// Update the ray end
			rayEnd -= RayDirection*(rayEndDistanceFromPlane/dot(ClipPlane_x_.xyz, RayDirection));
		}

		// Update maximum travel length along the ray
		float newLength = length(rayEnd - RayOrigin);

		// Security check: When something goes totally wrong and the maximum travel length along the ray
		// is increased instead of decreased we can overload the GPU which in worst case may result in
		// a total operation system stop, this has to be avoided, so no risk in here
		if (MaximumTravelLength > newLength)
			MaximumTravelLength = newLength;
	}
}
);	// STRINGIFY

// None-template version
static const PLCore::String sSourceCode_Fragment = sSourceCode_Fragment_Template + '\n' + STRINGIFY(
void ClipRay(inout float3 RayOrigin, float3 RayDirection, inout float MaximumTravelLength)
{
	// Call the function version also used by the template
	ClipRay_x_(RayOrigin, RayDirection, MaximumTravelLength);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
