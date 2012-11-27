/*!
\class CameraManager cameramanager.h <CameraManager>

Class that manages the cameras and stores direct access to the active camera

\sa Camera
*/

#include "cameramanager.h"
#include <QDebug>

namespace Cnoti3D
{
	/*!
	Constructs a new CameraManager with the given parent.
	*/
	CameraManager::CameraManager():
		_activeCamera(NULL)
	{

	}

	/*!
	Adds a camera to camera manager
	*/
	Camera* CameraManager::addCamera(Ogre::SceneManager* sceneManager, const QString& name )
	{
		Camera* cam;
		if ( (cam=camera(name))==NULL)
		{
			cam = new Camera(sceneManager, name.toStdString().c_str() );
			_cameraLst.insert(name, cam);
		}
		return cam;
	}

	/*!
	Returns a camera given the name
	*/
	Camera* CameraManager::camera( const QString& name )const
	{
		return _cameraLst.value(name,NULL);
	}

	/*!
	Returns the active camera
	*/
	Camera* CameraManager::activeCamera()const
	{
		return _activeCamera;
	}

	/*!
	Activates the camera and sets the aspect ratio
	*/
	Camera* CameraManager::setActiveCamera( const QString& name, Ogre::Viewport* vp)
	{
		qDebug() << "[CameraManager::setActiveCamera]" << vp->getActualWidth() << "x" << vp->getActualHeight();

		if(_activeCamera != NULL && name == _activeCamera->getName())
		{
			_activeCamera->setAspectRatio( vp->getActualWidth(), vp->getActualHeight() );
			return _activeCamera;
		}

		Camera* cam = NULL;
		if((cam=camera(name))!=NULL)
		{
			vp->setCamera( cam->getCamera() );
			cam->setAspectRatio( vp->getActualWidth(), vp->getActualHeight() );
			_activeCamera = cam;
		}
		return cam;
	}

	/*!
		Updates the aspect ratio for all cameras.
	*/
	void CameraManager::updateAllCamerasAspectRatio( qreal width, qreal height )
	{
		QHashIterator<QString,Camera*> it( _cameraLst );

		while(it.hasNext())
		{
			it.next();
			it.value()->setAspectRatio( Ogre::Real( width ), Ogre::Real( height ) );
		}
	}
}
