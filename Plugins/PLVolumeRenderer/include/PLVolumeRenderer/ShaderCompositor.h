/*********************************************************\
 *  File: ShaderCompositor.h                             *
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


#ifndef __PLVOLUMERENDERER_SHADERCOMPOSITOR_H__
#define __PLVOLUMERENDERER_SHADERCOMPOSITOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLVolumeRenderer/PLVolumeRenderer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Renderer;
}
namespace PLScene {
	class SQCull;
	class VisNode;
}
namespace PLVolumeRenderer {
	class ShaderFunction;
	class ShaderComposition;
	class ShaderCompositionID;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract shader compositor base class
*/
class ShaderCompositor : public PLCore::Object {


	// [TODO]
	friend class SRPVolume;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUMERENDERER_RTTI_EXPORT, ShaderCompositor, "PLVolumeRenderer", PLCore::Object, "Abstract shader compositor base class")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUMERENDERER_API virtual ~ShaderCompositor();

		// [TODO] Just a test
		PLVOLUMERENDERER_API ShaderFunction *GetShaderFunction(const PLCore::String &sClassName, PLCore::uint8 nNumOfClipPlanes = 0, PLCore::uint8 nNumOfDepthTextures = 0);

		/**
		*  @brief
		*    Clears the cache
		*
		*  @note
		*    - Destroys all cached shader composition instances
		*    - Destroys all cached shader function instances
		*/
		PLVOLUMERENDERER_API void ClearCache();

		/**
		*  @brief
		*    Returns a shader composition instance by using the provided shader composition ID
		*
		*  @param[in] cShaderCompositionID
		*    Shader composition ID
		*
		*  @return
		*    The shader composition instance, null pointer on error
		*/
		PLVOLUMERENDERER_API ShaderComposition *GetComposition(const ShaderCompositionID &cShaderCompositionID) const;


	//[-------------------------------------------------------]
	//[ Public virtual ShaderCompositor functions             ]
	//[-------------------------------------------------------]
	public:
		// [TODO] Just a test
		// Do not destroy the returnd instance, can be a null pointer
		virtual ShaderComposition *Generate(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLVOLUMERENDERER_API ShaderCompositor();

		/**
		*  @brief
		*    Creates a shader composition instance
		*
		*  @param[in] cShaderCompositionID
		*    Shader composition ID, must be valid!
		*
		*  @return
		*    The created shader composition instance, null pointer on error
		*/
		PLVOLUMERENDERER_API ShaderComposition *CreateShaderComposition(const ShaderCompositionID &cShaderCompositionID);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Array<ShaderFunction*>						m_lstShaderFunction;		/**< Shader function instances array */
		PLCore::HashMap<PLCore::String, ShaderFunction*>	m_mapShaderFunction;		/**< Shader function instances map */
		PLCore::Array<ShaderComposition*>					m_lstShaderCompositions;	/**< Shader compositions array (only valid shader composition are stored in here) */
		PLCore::HashMap<PLCore::uint32, ShaderComposition*> m_mapShaderCompositions;	/**< Shader compositions map (only valid shader composition are stored in here) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer


#endif // __PLVOLUMERENDERER_SHADERCOMPOSITOR_H__
