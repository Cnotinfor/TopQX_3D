#include "Button2d.h"
#include <QCursor>
#include <QGraphicsSceneEvent>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QPixmap>
#include <QApplication>
#include <QGraphicsScene>
#include <interface/qtCegui.h>
// CEGUI
#include <CEGUIImagesetManager.h>
#include <CEGUIWindowManager.h>
#include <CEGUIImageset.h>

namespace Cnoti3D {

int Button2d::_counter = 0;

/**
  Constructor for Button
 */
Button2d::Button2d(const QString &image, qreal x, qreal y, QObject *parent) :
	QObject(parent),
	_selected( false ),
	_imageNormal( image ),
	_imageHover( image ),
	_imageDown( image ),
	_imageDownHover( image )
{
	init(x, y, image);

	updateButtonImages();
}

Button2d::Button2d(const QString &image_normal, const QString &image_hover, const QString &image_down, qreal x, qreal y, QObject *parent) :
	QObject(parent),
	_selected( false ),
	_imageNormal( image_normal ),
	_imageHover( image_hover ),
	_imageDown( image_down ),
	_imageDownHover( image_down )
{
	init(x, y, image_normal);

	createImageSet(image_hover);
	createImageSet(image_down);

	updateButtonImages();
}

Button2d::Button2d(const QString &image_normal, const QString &image_hover, const QString &image_down, CEGUI::UDim x, CEGUI::UDim y, QObject *parent) :
	QObject(parent),
	_selected( false ),
	_imageNormal( image_normal ),
	_imageHover( image_hover ),
	_imageDown( image_down ),
	_imageDownHover( image_down )
{
	init(x, y, image_normal);

	createImageSet(image_hover);
	createImageSet(image_down);

	updateButtonImages();
}


Button2d::Button2d(const QString &image_normal, const QString &image_hover, const QString &image_down, const QString &image_downHover, qreal x, qreal y, QObject *parent) :
	QObject(parent),
	_selected( false ),
	_imageNormal( image_normal ),
	_imageHover( image_hover ),
	_imageDown( image_down ),
	_imageDownHover( image_downHover )
{
	init(x, y, image_normal);

	createImageSet(image_hover);
	createImageSet(image_down);
	createImageSet(image_downHover);

	updateButtonImages();
}

Button2d::Button2d(const QString &image_normal, const QString &image_hover, const QString &image_down, const QString &image_downHover, CEGUI::UDim x, CEGUI::UDim y, QObject *parent) :
	QObject(parent),
	_selected( false ),
	_imageNormal( image_normal ),
	_imageHover( image_hover ),
	_imageDown( image_down ),
	_imageDownHover( image_downHover )
{
	init(x, y, image_normal);

	createImageSet(image_hover);
	createImageSet(image_down);
	createImageSet(image_downHover);

	updateButtonImages();
}

void Button2d::setSelected(bool selected)
{
	if(selected == _selected)
	{
		return;
	}

	_selected = selected;

	updateButtonImages();
}

void Button2d::updateButtonImages()
{
	if(_selected)
	{
		setCeguiProperty("NormalImage", "set:" + _imageDown + " image:full_image");
		setCeguiProperty("HoverImage", "set:" + _imageDownHover + " image:full_image");
		setCeguiProperty("PushedImage", "set:" + _imageNormal + " image:full_image");
	}
	else
	{
		setCeguiProperty("NormalImage", "set:" + _imageNormal + " image:full_image");
		setCeguiProperty("HoverImage", "set:" + _imageHover + " image:full_image");
		setCeguiProperty("PushedImage", "set:" + _imageDown + " image:full_image");
	}
}

void Button2d::setCeguiProperty(const QString &property, const QString &value)
{
	try
	{
		_ceguiButton->setProperty( property.toStdString(), value.toStdString() );
	}
	catch (CEGUI::UnknownObjectException)
	{
		qWarning() << "[Button2d::setCeguiProperty] Property not found";
	}
	catch (CEGUI::InvalidRequestException)
	{
		qWarning() << "[Button2d::setCeguiProperty] Unable to interpret value content";
	}
}

/*!
	Init method, for every button created
*/
void Button2d::init( qreal x, qreal y, const QString &image )
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	_ceguiButton = (CEGUI::PushButton*)winMgr.createWindow("AquaLook/ImageButton", "bt_" + QString::number(_counter++).toStdString());

	// Position
	_ceguiButton->setPosition(CEGUI::UVector2(cegui_absdim(x), cegui_absdim(y)));

	// Size
	CEGUI::Imageset *btImageset = createImageSet(image);
	if(btImageset == NULL)
	{
		qWarning() << "[Button2d::init] Imageset is NULL, reseting size to 64x64";
		_ceguiButton->setSize(CEGUI::UVector2(cegui_absdim(64), cegui_absdim(64)));
	}
	else
	{
		CEGUI::Size s = btImageset->getNativeResolution();
		_ceguiButton->setSize(CEGUI::UVector2(cegui_absdim(s.d_width), cegui_absdim(s.d_height)));
	}

	// Uses click to make sure that button down occurred. (ex: problem when opening options after clicking a 3D button)
	_ceguiButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&Button2d::onMouseClick, this));
	_ceguiButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters, CEGUI::Event::Subscriber(&Button2d::onMouseEnters, this));
//	_ceguiButton->subscribeEvent(CEGUI::PushButton::EventMouseLeaves, CEGUI::Event::Subscriber(&Button2d::onMouseLeaves, this));

//	setCursor(QCursor(Qt::PointingHandCursor));
}

void Button2d::init( CEGUI::UDim x, CEGUI::UDim y, const QString &image )
{
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
	_ceguiButton = (CEGUI::PushButton*)winMgr.createWindow("AquaLook/ImageButton", "bt_" + QString::number(_counter++).toStdString());

	// Position
	_ceguiButton->setPosition(CEGUI::UVector2(x, y));

	// Size
	CEGUI::Imageset *btImageset = createImageSet(image);
	CEGUI::Size s = btImageset->getNativeResolution();
	_ceguiButton->setSize(CEGUI::UVector2(cegui_absdim(s.d_width), cegui_absdim(s.d_height)));

	_ceguiButton->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, CEGUI::Event::Subscriber(&Button2d::onMouseClick, this));
	_ceguiButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters, CEGUI::Event::Subscriber(&Button2d::onMouseEnters, this));

//	setCursor(QCursor(Qt::PointingHandCursor));
}


CEGUI::Imageset* Button2d::createImageSet(const QString &filename)
{
//	CEGUI::ImagesetManager::ImagesetIterator it(CEGUI::ImagesetManager::getSingleton().getIterator());
//	it.toStart();
//	while(!it.isAtEnd())
//	{
//		QString name(it.getCurrentValue()->getName().c_str());
//		if(name == filename)
//		{
//			return it.getCurrentValue();
//		}
//		it++;
//	}

	try
	{
		CEGUI::Imageset& imageset = CEGUI::ImagesetManager::getSingleton().createFromImageFile(filename.toStdString(), filename.toStdString(), "", CEGUI::XREA_RETURN);
		return (&imageset);
	}
//	catch (CEGUI::AlreadyExistsException)
//	{
//		qWarning() << "[Button2d::createImageSet] Already exists";
//	}
	catch (CEGUI::FileIOException)
	{
		qWarning() << "[Button2d::createImageSet] Error reading the file" << filename;

	}

	return NULL;
}

void Button2d::setToolTip(const QString& toolTip)
{
	_ceguiButton->setTooltipText( (CEGUI::utf8*) toolTip.toUtf8().data() );
}

void Button2d::setImageNormal(const QString &image)
{
	createImageSet(image);
	_imageNormal = image;
	updateButtonImages();
}

void Button2d::setImageHover(const QString &image)
{
	createImageSet(image);
	_imageHover = image;
	updateButtonImages();
}

void Button2d::setImageDown(const QString &image)
{
	createImageSet(image);
	_imageDown = image;
	updateButtonImages();
}

void Button2d::setImageDownHover(const QString &image)
{
	createImageSet(image);
	_imageDownHover = image;
	updateButtonImages();
}

void Button2d::setVisible(bool value)
{
	_ceguiButton->setVisible(value);
}

bool Button2d::isvisible()
{
	return _ceguiButton->isVisible();
}

void Button2d::show()
{
	setVisible(true);
}

void Button2d::hide()
{
	setVisible(false);
}

CEGUI::PushButton * Button2d::ceguiButton()
{
	return _ceguiButton;
}

/*             MOUSE EVENTS              */

bool Button2d::onMouseClick(const CEGUI::EventArgs& pEventArg)
{
//	qDebug() << "[Button2d::onClick]";
	CEGUI::MouseEventArgs mouseEvt = static_cast<const CEGUI::MouseEventArgs&>(pEventArg);
	if (mouseEvt.button == CEGUI::LeftButton)
	{
		emit clicked();
//		qDebug() << "[Button2d::onClick] Left button";
		return true;
	}
	return false;
}

//bool Button2d::onMouseLeaves(const CEGUI::EventArgs& pEventArg)
//{
//	return true;
//}

bool Button2d::onMouseEnters(const CEGUI::EventArgs& pEventArg)
{
//	_ogreWidget->setCursor(QCursor(Qt::PointingHandCursor));
	return true;
}


} // namespace Cnoti3D
