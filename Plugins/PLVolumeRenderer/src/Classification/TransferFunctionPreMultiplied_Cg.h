/*********************************************************\
 *  File: TransferFunctionPreMultiplied_Cg.h             *
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


// Pre-multiplied (no color bleeding) post-interpolative transfer function
// -> Is using pre-multiplied colors by their corresponding opacity value as described within the book "Real-Time Volume Graphics", section "3.2.3 Compositing" (page 54)


static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// Uniforms
uniform sampler1D TransferFunctionTexture;	// Transfer function texture map

/**
*  @brief
*    Scalar classification
*
*  @param[in] Scalar
*    Scalar to perform a classification on
*
*  @return
*    RGBA result of the classification
*/
float4 Classification(float Scalar)
{
	// Apply 1D transfer function
	float4 sample = tex1Dlod(TransferFunctionTexture, float4(Scalar, 0.0, 0.0, 0.0));

	// Revert pre-multiplication with opacity we did in order to avoid color bleeding during interpolation
	sample.rgb *= 1.0/sample.a;

	// [TODO] Only required for HDR rendering with gamma correction -> Add a trigger
	// [TODO] For better performance, put this directly within the 1D texture
	// Perform sRGB to linear space conversion (gamma correction)
	sample.rgb = pow(sample.rgb, vec3(2.2, 2.2, 2.2));

	// Return the result of the scalar classification
	return sample;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
