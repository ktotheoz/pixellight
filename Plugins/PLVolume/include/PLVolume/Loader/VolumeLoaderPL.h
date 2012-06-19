/*********************************************************\
 *  File: VolumeLoaderPL.h                               *
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


#ifndef __PLVOLUME_VOLUMELOADER_PL_H__
#define __PLVOLUME_VOLUMELOADER_PL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLVolume/Loader/VolumeLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Volume;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Volume loader implementation for the "volume" volume data XML file format of PixelLight
*
*  @remarks
*    Example:
*  @verbatim
*    <?xml version="1.0" ?>
*    <Volume Version="1">
*       <VolumeFilename>Thomas.dds</VolumeFilename>
*       <VoxelSize>0.0048828130 0.0048828130 0.025000002</VoxelSize>
*       <TransferFunctionFilename>TestTransferFunction.table</TransferFunctionFilename>
*    </Volume>
*  @endverbatim
*/
class VolumeLoaderPL : public VolumeLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUME_RTTI_EXPORT, VolumeLoaderPL, "PLVolume", PLVolume::VolumeLoader, "Volume loader implementation for the \"volume\" volume data XML file format of PixelLight")
		// Properties
		pl_properties
			pl_property("Formats",	"volume,VOLUME")
			pl_property("Load",		"1")
			pl_property("Save",		"1")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_2(Load,	pl_ret_type(bool),	Volume&,		PLCore::File&,	"Load method",	"")
		pl_method_2(Save,	pl_ret_type(bool),	const Volume&,	PLCore::File&,	"Save method",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		bool Load(Volume &cVolume, PLCore::File &cFile);
		bool Save(const Volume &cVolume, PLCore::File &cFile);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		VolumeLoaderPL();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VolumeLoaderPL();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Loader implementation for format version 1
		*
		*  @param[in] cVolume
		*    Volume to load into
		*  @param[in] cFile
		*    File we're loading from
		*  @param[in] cVolumeElement
		*    Volume XML element to read the data from
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool LoadV1(Volume &cVolume, PLCore::File &cFile, const PLCore::XmlElement &cVolumeElement) const;

		/**
		*  @brief
		*    Constructs an absolute filename
		*
		*  @param[in] cFile
		*    Currently used file
		*  @param[in] sFilename
		*    Filename to get an absolute filename from
		*
		*  @return
		*    Absolute filename
		*/
		PLCore::String GetAbsoluteFilename(const PLCore::File &cFile, const PLCore::String &sFilename) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_VOLUMELOADER_PL_H__
