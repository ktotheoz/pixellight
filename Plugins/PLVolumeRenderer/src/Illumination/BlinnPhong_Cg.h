/*********************************************************\
 *  File: BlinnPhong_Cg.h                                *
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
uniform float3 LightColor;	// Light color

/**
*  @brief
*    Illumination
*
*  @param[in] SurfaceColor
*    Surface color
*  @param[in] SurfaceNormal
*    Normalized surface normal
*  @param[in] ViewingDirection
*    Normalized viewing direction
*  @param[in] LightDirection
*    Normalized light direction
*
*  @return
*    Illumination result
*/
float3 Illumination(float3 SurfaceColor, float3 SurfaceNormal, float3 ViewingDirection, float3 LightDirection)
{
	// [TODO] Cleanup (from the book "Real-Time Volume Graphics")

	// material properties
	float3 Kd = SurfaceColor; // diffuse
	float3 Ks = float3(0.2, 0.2, 0.2); // specular
	float shininess = 100.0; // shininess

	// Calculate halfway vector
	float3 H = normalize(LightDirection + ViewingDirection);

	// Compute the diffuse term
	float diffuseLight = max(dot(LightDirection, SurfaceNormal), 0);
	float3 diffuse = Kd * LightColor * diffuseLight;

	// Compute the specular term
	float specularLight = pow(max(dot(H, SurfaceNormal), 0), shininess);
	if (diffuseLight <= 0) specularLight = 0;
	float3 specular = Ks * LightColor * specularLight;
	return diffuse + specular;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY