/*********************************************************\
 *  File: TransferFunctionWidget.h                       *
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


#ifndef __PLVOLUMEGUI_TRANSFERFUNCTIONWIDGET_H__
#define __PLVOLUMEGUI_TRANSFERFUNCTIONWIDGET_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/PLCore.h>
PL_WARNING_PUSH
	PL_WARNING_DISABLE(4127)	// "warning C4127: conditional expression is constant"
	PL_WARNING_DISABLE(4251)	// "'QPainterPath::d_ptr' : class 'QScopedPointer<T,Cleanup>' needs to have dll-interface to be used by clients of class 'QPainterPath'"
	PL_WARNING_DISABLE(4800)	// "'const quint32' : forcing value to bool 'true' or 'false' (performance warning)"
	PL_WARNING_DISABLE(4530)	// "C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc"
	#include <QtCore/qpoint.h>
	#include <QtGui/qlabel.h>
PL_WARNING_POP


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Image;
}
namespace PLVolume {
	class Volume;
	class Histogram;
}
namespace PLVolumeGui {
	class DockWidgetVolumeTransferFunction;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolumeGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Transfer function Qt widget
*/
class TransferFunctionWidget : public QLabel {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cDockWidgetVolumeTransferFunction
		*    Dock widget volume transfer function
		*  @param[in] cVolume
		*    Selected volume instance
		*  @param[in] pQWidgetParent
		*    Pointer to parent Qt widget as parameter, can be a null pointer
		*/
		TransferFunctionWidget(DockWidgetVolumeTransferFunction &cDockWidgetVolumeTransferFunction, PLVolume::Volume &cVolume, QWidget *pQWidgetParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TransferFunctionWidget();


	//[-------------------------------------------------------]
	//[ Protected virtual QWidget functions                   ]
	//[-------------------------------------------------------]
	protected:
		virtual void mousePressEvent(QMouseEvent *) override;
		virtual void mouseReleaseEvent(QMouseEvent *) override;
		virtual void mouseMoveEvent(QMouseEvent *) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Updates the composed image
		*/
		void UpdateComposedImage();

		/**
		*  @brief
		*    Updates the Qt pixmap by using the content of the composed image
		*/
		void UpdatePixmap();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DockWidgetVolumeTransferFunction *m_pDockWidgetVolumeTransferFunction;	// [TODO] Remove this
		PLVolume::Volume	*m_pVolume;					/**< Selected volume instance, always valid */
		PLVolume::Histogram	*m_pHistogram;				/**< Histogram, can be a null pointer (data is derived from the currently selected volume) */
		PLGraphics::Image	*m_pHistogramImage;			/**< Histogram as image, can be a null pointer (data is derived from "m_pHistogram") */
		PLGraphics::Image	*m_pComposedImage;			/**< Composed image, can be a null pointer */
		int					 m_nSelectedChannel;		/**< Currently selected channel, <0 if nothing selected */
		int					 m_nSelectedControlPoint;	/**< Currently selected control point, <0 if nothing selected */
		bool				 m_bLeftMouseButtonDown;	/**< Is the left mouse button currently down? */
		QPoint				 m_vPreviousMousePosition;	/**< Previous mouse position (used when the left mouse button currently down) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeGui


#endif // __PLVOLUMEGUI_TRANSFERFUNCTIONWIDGET_H__
