/*!
\class Camera camera.h <Camera>

This class provides the interface with Ogre's Cameras.

\sa CameraManager
*/
#include "camera.h"
#include "UtilFunctions.h"
// Qt
#include <QPropertyAnimation>
#include <QDebug>

namespace Cnoti3D
{

	/*!
	Constructs a new Camera.
	\param sceneMgr Camera's SceneManager
	\param name     Camera's name
	*/
	Camera::Camera(Ogre::SceneManager *sceneMgr,
				   const QString& name)
	{
		QVector3D pos(0,50,500);
		QVector3D target(0,0,0);

		init(sceneMgr, name, pos, target );
	}

	/*!
	Constructs a new Camera.
	\param sceneMgr     Camera's SceneManager
	\param name         Camera's name
	\param pos          Camera's position
	\param target       Camera's target
	*/
	Camera::Camera(Ogre::SceneManager *sceneMgr,
				   const QString& name,
				   QVector3D &pos,
				   QVector3D &target)
	{
		init(sceneMgr, name, pos, target);
	}

	/*!
	Initializes the new camera information.
	\param sceneMgr     Camera's SceneManager
	\param name         Camera's name
	\param pos          Camera's position
	\param target       Camera's target
	*/
	void Camera::init( Ogre::SceneManager *sceneMgr,
					   const QString& name,
					   QVector3D &pos,
					   QVector3D &target)
	{
		_cameraNode	= NULL;
		_targetNode	= NULL;
		_camera		= NULL;

		// Basic member references setup
		//_name = name;

		if(sceneMgr->hasSceneNode(name.toStdString()))
		{
			_cameraNode = sceneMgr->getSceneNode( name.toStdString() );
			_camera = sceneMgr->getCamera( name.toStdString() );
			if(sceneMgr->hasSceneNode(name.toStdString() + ".Target"))
			{
				_targetNode = sceneMgr->getSceneNode( name.toStdString() + ".Target" );
			}
			else
			{
				_targetNode = sceneMgr->getRootSceneNode()->createChildSceneNode( name.toStdString() + ".Target" );
			}
		}
		else
		{
			//create camera
			_camera = sceneMgr->createCamera( name.toStdString() );

			//default camera
			_camera->setNearClipDistance( 5.0f );
			_camera->setPosition( 0, 0, 0 );	// Camera position is set by node
			_camera->lookAt( 0, 0, 0 );			// Target position is set by node
			_camera->setFarClipDistance( 1500.0f );

			// Create the camera's node
			_cameraNode = sceneMgr->getRootSceneNode()->createChildSceneNode( name.toStdString() );
			_cameraNode->attachObject( _camera );
			_cameraNode->setPosition(UtilFunctions::qVector3dToOgreVector3(pos));
			// Create the camera's target node
			_targetNode = sceneMgr->getRootSceneNode()->createChildSceneNode( name.toStdString() + ".Target" );
			_targetNode->setPosition(UtilFunctions::qVector3dToOgreVector3(target));
		}

		_cameraNode->setAutoTracking( true, _targetNode ); // The camera will always look at the camera target
		_cameraNode->setFixedYawAxis( true );		// Needed because of auto tracking

		_defaultPosition = UtilFunctions::ogreVector3ToQVector3d(_cameraNode->getPosition());
		_position = _defaultPosition;
		_defaultTargetPosition = UtilFunctions::ogreVector3ToQVector3d(_targetNode->getPosition());
		_targetPosition = _defaultTargetPosition;

		// movement
		_animation = new QPropertyAnimation(this, "position");
		_targetAnimation = new QPropertyAnimation(this, "targetPosition");
		connect(_animation, SIGNAL(finished()), this, SLOT(stopMovingTasks()));

		// Zoom
		_hasZoomApplied = false;
		_zoomOffTimerId = 0;
		_zoomTime       = 0;
	}

	/*!
	Moves the Camera to it's default position.
	\param newPos     Camera's new position
	\param time       Time (milliseconds) that takes to move to the new position
	*/
	void Camera::moveTo(float time /*= 1 */)
	{
		moveTo(_defaultPosition, _defaultTargetPosition, time);
	}

	/*!
	Moves the Camera.
	\param newPos     Camera's new position
	\param time       Time (milliseconds) that takes to move to the new position
	*/
	void Camera::moveTo(QVector3D newPos, QVector3D newTargetPos, float time /*= 1 */)
	{
		// Animation
		_animation->setDuration(time);
		_animation->setStartValue(_position);
		_animation->setEndValue(newPos);

		_targetAnimation->setDuration(time);
		_targetAnimation->setStartValue(_targetPosition);
		_targetAnimation->setEndValue(newTargetPos);

		_animation->start();
		_targetAnimation->start();
	}

	/*!
	  Stops the camera movement
	*/
	void Camera::stopMoving()
	{
		//
		if(_animation->state() == QAbstractAnimation::Running)
		{
			_animation->stop();
		}
		else
		{
			stopMovingTasks();
		}
	}

	/*!
	  Checks if some zoom is aplied to the camera and emits the stopedMoving signal.
	*/
	void Camera::stopMovingTasks()
	{
		qreal dist = UtilFunctions::calculateDistance(_position, _defaultPosition);
		// Check if is in default position
		if(dist < 0.0001f)
		{
			_hasZoomApplied = false;
		}
		else
		{
			_hasZoomApplied = true;
		}

		emit stoppedMoving();
	}

	/*!
	Zooms in (moves) closer to an object.
	\param sceneNode    Object sceneNode
	\param time         Time (milliseconds) that takes to move to the new position
	*/
	void Camera::zoomInToObject(Ogre::SceneNode* sceneNode, float time)
	{
		if (_zoomOffTimerId != 0)
		{
			killTimer(_zoomOffTimerId);
		}

		QVector3D nodePos =  UtilFunctions::ogreVector3ToQVector3d(sceneNode->getPosition());
		QVector3D direction = _position - nodePos;
		direction.normalize();
		QVector3D position = nodePos/* + direction * 1000*/;
		position.setY(nodePos.y() + 450);
		position.setZ(nodePos.z() + 800);
		QVector3D targetPos = nodePos;
		targetPos.setZ(position.z() - 1000);
		this->moveTo(position, targetPos, time);
	}

	/*!
	Zooms ou (moves) to default position.
	\param time         Time (milliseconds) that takes to move to the new position
	\param time         Time (milliseconds) until it will start to move
	*/
	void Camera::zoomOff(float time, int wait)
	{
		if (_zoomOffTimerId != 0 || !_hasZoomApplied)
		{
			return;
		}

		if (wait == 0)
		{
			this->moveTo(time);
		}
		else
		{
			_zoomTime = time;
			_zoomOffTimerId = startTimer(wait);
		}
	}

	void Camera::timerEvent(QTimerEvent *event)
	{
		if (event->timerId() == _zoomOffTimerId)
		{
			killTimer(_zoomOffTimerId);
			_zoomOffTimerId = 0;
			this->moveTo(_zoomTime);
			_zoomTime = 0;
		}
	}

	/*
	+----------------------------------------------+
	|                  Setters                     |
	+----------------------------------------------+
	*/
	/*!
	Sets Camera's aspect ratio
	*/
	void Camera::setAspectRatio(qreal x, qreal y)
	{
		Ogre::Real aspectRatio = x / y ;
		_camera->setAspectRatio( aspectRatio );
	}

	/*!
	Sets Camera's position
	*/
	void Camera::setPosition(float x, float y, float z)
	{
		QVector3D p = QVector3D(x, y, z);
		this->setPosition( p );
	}

	/*!
	Sets Camera's position
	*/
	void Camera::setPosition(QVector3D pos)
	{
		_position = pos;
		_cameraNode->setPosition(UtilFunctions::qVector3dToOgreVector3(_position));
	}

	/*!
	Sets Camera's target
	*/
	void Camera::setTarget( float x, float y, float z )
	{
		QVector3D p = QVector3D( x, y, z );
		this->setTarget( p );
	}

	/*!
	Sets Camera's target
	*/
	void Camera::setTarget( QVector3D pos )
	{
		_targetNode->setPosition( UtilFunctions::qVector3dToOgreVector3(pos) );
		_targetPosition = pos;
	}

	/*
	+----------------------------------------------+
	|                  Getters                     |
	+----------------------------------------------+
	*/
	/*!
	Gets Camera's node. The Camera is attached to this node, if the node is moved the camera will also move.
	*/
	Ogre::SceneNode* Camera::getNode()
	{
		return _cameraNode;
	}

	/*!
	Gets Camera's target node. The Camera's target is attached to this node, if the node is moved the camera's target will also move.
	*/
	Ogre::SceneNode* Camera::getTargetNode()
	{
		return _targetNode;
	}

	/*!
	Gets the Camera
	*/
	Ogre::Camera* Camera::getCamera()
	{
		return _camera;
	}

	/*!
	Gets a world space ray as cast from the camera through a viewport position.
	*/
	Ogre::Ray Camera::getCameraToViewportRay( float x, float y )
	{
		return _camera->getCameraToViewportRay( x, y );
	}

	/*!
	Gets Camera's position
	*/
	QVector3D Camera::getPosition()
	{
		return _position;
	}

	/*!
	Gets Camera's name
	*/
	QString Camera::getName()
	{
		return _camera->getName().c_str();
	}

}
