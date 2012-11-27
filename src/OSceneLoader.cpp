/*!
\class OSceneLoader osceneloader.h <OSceneLoader>
This class loads an initilizes the 3D exported objects

\sa OSceneCallback,  OSceneManager
*/
#include "stdafx.h"
#include "oSceneLoader/include/OgreOSMScene.h"
#include "OSceneCallback.h"
#include "OSceneLoader.h"

namespace Cnoti3D
{
	OSceneLoader::OSceneLoader()
	{
		_OSMScene = NULL;
		_callbacks = NULL;
	}

	OSceneLoader::OSceneLoader( Ogre::SceneManager *sceneMgr, Ogre::RenderWindow *window, const QString name, OSceneCallback *callBacks )
	{
		_OSMScene = NULL;
		_callbacks = NULL;

		_osmSceneName = name;

		// Alloc for objects
		_OSMScene = new OSMScene( sceneMgr, window );
		_callbacks = new OSceneCallback();

		// Initialize scene
		_OSMScene->initialise( name.toStdString().c_str(), callBacks );

		// declare resources
		_OSMScene->declareResources();
	}

	OSceneLoader::~OSceneLoader()
	{
		if( _OSMScene != NULL )
		{
			delete _OSMScene;
			_OSMScene = NULL;
		}

		if( _callbacks != NULL )
		{
			delete _callbacks;
			_callbacks = NULL;
		}
	}

	bool OSceneLoader::loadScene()
	{
		// Create and setup our scene in the root node
		try
		{
			if( !_OSMScene->createScene() ) 
			{
				return false;
			}
		}
		catch( Ogre::ItemIdentityException* e )
		{
			Ogre::LogManager::getSingleton().logMessage( e->getFullDescription().c_str() );
			return false;
		}

		return true;
	}

/*
+----------------------------------------------+
|                  Getters                     |
+----------------------------------------------+
*/
	const QString& OSceneLoader::osmSceneName()
	{
		return _osmSceneName;
	}

	OSMScene::CameraList& OSceneLoader::getCameraList()
	{
		return _OSMScene->getCameraList();
	}

	OSMScene::EntityList& OSceneLoader::getEntityList()
	{
		return _OSMScene->getEntityList();
	}

	OSMScene::HelperList& OSceneLoader::getHelperList()
	{
		return _OSMScene->getHelperList();
	}

	OSMScene::LightList& OSceneLoader::getLightList()
	{
		return _OSMScene->getLightList();
	}
}
