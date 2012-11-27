/*!
\class Object3DMousable Object3dMousable.h <Object3DMousable>

Implements a Object3D which is mouse aware.

\sa Object3D, Object3DAnimated, Object3DAnimatedComplex
*/
#include "Object3dMousable.h"
// CEGUI
#include <CEGUISystem.h>
#include <CEGUIWindow.h>
// Qt
#include <QToolTip>
#include <QPoint>
#include <QTimeLine>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>

namespace Cnoti3D
{
	/*!
	Constructs a new Object3D.
	\param sceneManager     Ogre's SceneManager
	\param objectName       Object3D's name
	*/
	Object3DMousable::Object3DMousable( Ogre::SceneManager* sceneManager, const QString& objectName ):
		Object3D( sceneManager, objectName ),
		_enable( true ),
		_tooltipTimer( 0 ),
		_tooltipPos(),
		_tooltipText(),
		_rotateOnMouseHover( false ),
		_animationTimer( 0 ),
		_boundBoxClickable( true )
	{

	}

	/*!
	Constructs a new Object3D.
	\param entity   Ogre's entity
	*/
	Object3DMousable::Object3DMousable( Ogre::Entity* entity ):
		Object3D(entity),
		_enable( true ),
		_tooltipTimer( 0 ),
		_tooltipPos(),
		_tooltipText(),
		_angle( 0 ),
		_rotateOnMouseHover( false ),
		_animationTimer( 0 ),
		_boundBoxClickable( true )
	{

	}

	void Object3DMousable::addNode(const QString& entityName)
	{
		Ogre::SceneNode* node;
		if( _sceneManager->hasSceneNode( entityName.toStdString() ) )
		{
			node = _sceneManager->getSceneNode( entityName.toStdString() );
			Ogre::SceneNode* pNode = _sceneManager->getSceneNode(name().toStdString());
			node->getParent()->removeChild(node->getName());
			pNode->addChild(node);
			node->setPosition(0,0,0);
		}
		else
		{
			qWarning()<< "[Object3DMousable::addNode]:The node (" + entityName + ") doesn't exist.";
		}
	}

	void Object3DMousable::mouseLeave()
	{
		stopRotation();
	}


	void Object3DMousable::startRotation()
	{
		//qDebug() << "[Object3DMousable::startToRotate]";
		_animationTimer = startTimer(30); // more or less 30 frames per second
	}

	void Object3DMousable::stopRotation()
	{
		if( _animationTimer != 0)
		{
			killTimer(_animationTimer);
			_animationTimer = 0;
		}
	}

	void Object3DMousable::setBoundBoxClickable(bool value)
	{
		_boundBoxClickable = value;
	}

	bool Object3DMousable::isBoundBoxClickable()
	{
		return _boundBoxClickable;
	}

	/*
	This event handler, for event mouseEvent, can be reimplemented in a subclass to receive mouse doubleclick events.
	*/
	void Object3DMousable::onMouseDoubleClickEvent ( QMouseEvent * mouseEvent )
	{
	}

	/*
	This event handler, for event mouseEvent, can be reimplemented in a subclass to receive mouse events.
	*/
	void Object3DMousable::onMouseMoveEvent ( QMouseEvent * mouseEvent )
	{

	}

	/*
	This event handler, for event mouseEvent, can be reimplemented in a subclass to receive mouse press events.
	*/
	void Object3DMousable::onMousePressEvent ( QMouseEvent * mouseEvent )
	{
	}

	/*
	This event handler, for event mouseEvent, can be reimplemented in a subclass to receive mouse release events.
	*/
	void Object3DMousable::onMouseReleaseEvent ( QMouseEvent * mouseEvent )
	{
		qDebug() << "[Object3DMousable::onMouseReleaseEvent] - Emited clicked";
		if (!_tooltipText.isEmpty())
		{
			CEGUI::System::getSingleton().getGUISheet()->setTooltipText("");
		}
		emit clicked();
        mouseEvent->accept();
	}

	/*
	This event handler, for event mouseEvent, can be reimplemented in a subclass to receive mouse wheel events.
	*/
	void Object3DMousable::onWheelEvent ( QWheelEvent * wheelEvent )
	{
	}

	/*
	This event handler, for event mouseEvent, can be reimplemented in a subclass to receive mouse enter event.
	*/
	void Object3DMousable::onMouseEnterEvent( QMouseEvent* mouseEvent )
	{
		if( _rotateOnMouseHover )
		{
			startRotation();
		}
		if (!_tooltipText.isEmpty())
		{
			CEGUI::System::getSingleton().getGUISheet()->setTooltipText((CEGUI::utf8*) _tooltipText.toUtf8().data());
		}
	}

	/*
	This event handler, for event mouseEvent, can be reimplemented in a subclass to receive mouse enter event.
	*/
	void Object3DMousable::onMouseLeaveEvent( QMouseEvent* mouseEvent )
	{
		if( _rotateOnMouseHover )
		{
			stopRotation();
		}
		if (!_tooltipText.isEmpty())
		{
			CEGUI::System::getSingleton().getGUISheet()->setTooltipText("");
		}
	}

	void Object3DMousable::timerEvent(QTimerEvent *event)
	{
		if(event->timerId() == _animationTimer)
		{
			setAngle(10);
		}
	}

	void Object3DMousable::setAngle(int angle)
	{
		node()->yaw( Ogre::Radian(Ogre::Degree(angle)) );
		_angle = angle;
	}
}

