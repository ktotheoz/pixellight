/*********************************************************\
 *  File: DockWidgetVolume.cpp                           *
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
#include "PLVolumeGui/DockWidgetVolume.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLVolumeGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(DockWidgetVolume)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the currently selected object
*/
Object *DockWidgetVolume::GetSelectedObject() const
{
	return m_pObject;
}

/**
*  @brief
*    Selects the given object
*/
void DockWidgetVolume::SelectObject(Object *pObject)
{
	// State change?
	if (m_pObject != pObject) {
		// Disconnect event handler
		if (m_pObject) {
			m_pObject->SignalDestroyed.Disconnect(SlotOnDestroyed);
			m_pObject = nullptr;
		}

		// We're only interested in volume scene nodes
		if (pObject && pObject->IsInstanceOf("PLVolume::SNVolume")) {
			// Backup the given object pointer
			m_pObject = pObject;

			// Connect event handler
			if (m_pObject)
				m_pObject->SignalDestroyed.Connect(SlotOnDestroyed);
		}
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DockWidgetVolume::DockWidgetVolume(QWidget *pQWidgetParent, PLFrontendQt::DockWidgetManager *pDockWidgetManager) : DockWidget(pQWidgetParent, pDockWidgetManager),
	SlotOnDestroyed(this),
	m_pObject(nullptr)
{
	// Ask the RTTI dock widget fellows whether or not someone knows which is the currently selected object

	// Get a list of dock widgets registered within the same dock widget manager this dock widget is in
	const Array<DockWidget*> &lstDockWidgets = GetFellowDockWidgets();
	Object *pObject = nullptr;
	for (uint32 i=0; i<lstDockWidgets.GetNumOfElements() && !pObject; i++) {
		// Get the dock widget, and ignore our own ego
		DockWidget *pDockWidget = lstDockWidgets[i];
		if (pDockWidget != this) {
			// Get the typed dynamic parameters
			Params<Object*> cParams;

			// Call the RTTI method
			pDockWidget->CallMethod("GetSelectedObject", cParams);

			// Get the result, we're interested in any object
			pObject = cParams.Return;
			if (pObject)
				SelectObject(pObject);
		}
	}
}

/**
*  @brief
*    Destructor
*/
DockWidgetVolume::~DockWidgetVolume()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the object assigned with this dock widget was destroyed
*/
void DockWidgetVolume::OnDestroyed()
{
	// Argh! Mayday! We lost our object!
	// -> Now no object is currently selected
	SelectObject(nullptr);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeGui
