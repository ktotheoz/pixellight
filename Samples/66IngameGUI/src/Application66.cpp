/*********************************************************\
 *  File: Application66.cpp                              *
 *
 *  Copyright (C) 2002-2011 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGui/Gui/Gui.h>
#include <PLGui/Gui/Resources/Font.h>
#include <PLInput/Input/Controller.h>
#include <PLInput/Input/Controls/Control.h>
#include <PLScene/Scene/SPScene.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLFrontendPLGui/Compositing/SNGui.h>
#include "IngameGui.h"
#include "Application66.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGui;
using namespace PLInput;
using namespace PLRenderer;
using namespace PLScene;
using namespace PLEngine;
using namespace PLFrontendPLGui;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Application66)


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
Font *Application66::GuiFont = nullptr;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application66::Application66(Frontend &cFrontend) : EngineApplication(cFrontend),
	SlotOnControl(this),
	m_pIngameGui(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
Application66::~Application66()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a control event has occurred
*/
void Application66::OnControl(Control &cControl)
{
	// Check whether the escape key was pressed
	if (cControl.GetType() == ControlButton && cControl.GetName() == "KeyboardEscape")
		Exit(0); // Shut down the application
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::CoreApplication functions   ]
//[-------------------------------------------------------]
void Application66::OnDeInit()
{
	// Destroy the GUI font
	if (GuiFont) {
		delete GuiFont;
		GuiFont = nullptr;
	}

	// Call base implementation
	EngineApplication::OnDeInit();
}


//[-------------------------------------------------------]
//[ Private virtual PLEngine::EngineApplication functions ]
//[-------------------------------------------------------]
void Application66::OnCreateScene(SceneContainer &cContainer)
{
	// Set scene container flags
	cContainer.SetFlags(SceneNode::NoCulling | SceneNode::NoPause);

	// Setup scene surface painter
	SurfacePainter *pPainter = GetPainter();
	if (pPainter && pPainter->IsInstanceOf("PLScene::SPScene")) {
		SPScene *pSPScene = static_cast<SPScene*>(pPainter);
		pSPScene->SetRootContainer(cContainer.GetContainer());
		pSPScene->SetSceneContainer(&cContainer);
	}

	// Within the parent container...
	SceneContainer *pContainer = cContainer.GetContainer();
	if (pContainer) {
		// Create a 'ingame'-GUI scene node
		const SNGui *pGuiSceneNode = static_cast<SNGui*>(pContainer->Create("PLFrontendPLGui::SNGui", "GUI"));
		if (pGuiSceneNode) {
			// Setup the GUI
			Gui *pGui = pGuiSceneNode->GetGui();
			if (pGui) {
				// Create the GUI font
				if (!GuiFont) {
					GuiFont = new Font(*pGui);
					GuiFont->LoadFromFile("Data/Fonts/LinLibertine_Re-2.7.9.9.otf", 18);
				}

				// Set GUI options
				pGui->SetMouseVisible(false);

				// Create ingame GUI container
				m_pIngameGui = new IngameGui(*this, pGui->GetRootWidget());
				m_pIngameGui->SetSize(Vector2i(1024, 768));
				m_pIngameGui->SetBackgroundColor(PLGraphics::Color4::Transparent);
				m_pIngameGui->SetFocus();
				m_pIngameGui->SetVisible(true);
			}
		}
	}

	// Set scene container
	SetScene(&cContainer);
}

void Application66::OnCreateInputController()
{
	// Call base implementation
	EngineApplication::OnCreateInputController();

	// Get virtual input controller
	Controller *pController = reinterpret_cast<Controller*>(GetInputController());
	if (pController)
		pController->SignalOnControl.Connect(SlotOnControl);
}