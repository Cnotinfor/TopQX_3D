#ifndef BUTTON_H
#define BUTTON_H
/**

  Class for making buttons in QGraphicsScene.

  It emits a signal when it is clicked.

  It also changes the images and mouse cursor on hover and click, if the image is specified.

 */
#include "Cnoti3d_global.h"
// CEGUI
#include <elements/CEGUIPushButton.h>
// Qt
#include <QGraphicsObject>
#include <QString>

class QPixmap;

namespace Cnoti3D {

class CNOTI3DLIB_EXPORT Button2d : public QObject
{
	Q_OBJECT
public:
	explicit Button2d(const QString &image, qreal x, qreal y, QObject *parent = 0);
	explicit Button2d(const QString &image_normal, const QString &image_hover, const QString &image_down, qreal x, qreal y, QObject *parent = 0);
	explicit Button2d(const QString &image_normal, const QString &image_hover, const QString &image_down, CEGUI::UDim x, CEGUI::UDim y, QObject *parent = 0);
	explicit Button2d(const QString &image_normal, const QString &image_hover, const QString &image_down, const QString &image_downHover, qreal x, qreal y, QObject *parent = 0);
	explicit Button2d(const QString &image_normal, const QString &image_hover, const QString &image_down, const QString &image_downHover, CEGUI::UDim x, CEGUI::UDim y, QObject *parent = 0);

	void setSelected(bool selected);
	void setImageNormal( const QString &image );
	void setImageHover( const QString &image );
	void setImageDown( const QString &image );
	void setImageDownHover( const QString &image );
	void setVisible(bool value);
	bool isvisible();
	void show();
	void hide();

	void updateButtonImages();
signals:
	/**
		Signal emited when the button is pressed
	*/
	void clicked();
private:
	// Private methods
	void init( qreal x, qreal y, const QString &image );
	void init( CEGUI::UDim x, CEGUI::UDim y, const QString &image );
	// Private attributes
	bool _selected;
	QString _imageNormal;
	QString _imageHover;
	QString _imageDown;
	QString _imageDownHover;
// CEGUI
public:
	CEGUI::PushButton* ceguiButton();
	void setToolTip(const QString &toolTip);
protected:
	bool onMouseClick(const CEGUI::EventArgs& pEventArg);
//	bool onMouseLeaves(const CEGUI::EventArgs &pEventArg);
	bool onMouseEnters(const CEGUI::EventArgs &pEventArg);
	void setCeguiProperty(const QString &property, const QString &value);
private:
	static int _counter;
	CEGUI::PushButton* _ceguiButton;
	//methods
	CEGUI::Imageset* createImageSet(const QString &filename);
};

} // namespace Cnoti3D

#endif // BUTTON_H
