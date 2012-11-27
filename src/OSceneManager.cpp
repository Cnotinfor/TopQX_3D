/*! 
	\class OSceneManager OSceneManager.h "OSceneManager.h"
	\brief Manage osm scene files.

	This class holds an manages the information of the osm scene files loaded.

	Example:
	\code

	OSceneManager* oSceneManager = new OSceneManager();
	oSceneManager->loadSceneFromFile( "my_OSM_file.osm", mySceneManager_pointer, myOgreRenderWindow_pointer );

	\endcode

	Logs are written in the Ogre log.

	\sa OSceneLoader

	Copyright (c) 2009 by Cnotinfor
*/

#include "OSceneManager.h"
#include "OSceneLoader.h"
// Qt
#include <QString>

namespace Cnoti3D
{
/*
+----------------------------------------------+
|              {Des,Cons}tructors              |
+----------------------------------------------+
*/

	/*!
		Constructor for the class OSceneManager.
	*/
	OSceneManager::OSceneManager()
	{
		_lastOSceneLoaded = NULL;
	}

	/*!
		Destructor for the class OSceneManager.
	*/
	OSceneManager::~OSceneManager()
	{
		// delete scenes
		for( int i = 0; i < _oSceneLoadedList.size() ; i++ )
		{
			delete( _oSceneLoadedList[i] );
		}
		_oSceneLoadedList.clear();
	}

/*
+----------------------------------------------+
|              Complex Functions               |
+----------------------------------------------+
*/

	/*!
		Loads a scene from the file \a filename<BR>
		By default we use our own scene manager, but we can feed any other by \a sceneMgr if needed.<BR>
		<BR>
		Returns true if a scene was loaded, otherwise false.
	*/
	bool OSceneManager::loadSceneFromFile( QString sceneName, Ogre::SceneManager* sceneMgr, Ogre::RenderWindow* renderWindow )
	{
		if( !sceneMgr || !renderWindow )
		{
			Ogre::LogManager::getSingleton().logMessage( "SceneManager or RenderWindow is not valid." );
			return false;
		}
		//
		//	Create loader
		//
		OSceneLoader* s = NULL;
		try
		{			
			s = new OSceneLoader( sceneMgr, renderWindow, sceneName.toStdString().c_str() );
		}
		catch( ... )
		{
			Ogre::LogManager::getSingleton().logMessage( "Exception: Error creating OSceneLoader." );
			return false;
		}
		//
		//	Check if loader was created
		//
		if( !s )
		{
			Ogre::LogManager::getSingleton().logMessage( "Error creating OSceneLoader." );
			return false;
		}
		//
		//	Load the scene
		//
		if( !s->loadScene() )
		{
			return false;
		}
		//
		// add scene to our list
		//
		_oSceneLoadedList.push_back( s );
		//
		// keep track on last scene loaded
		//
		_lastOSceneLoaded = s;

		return true;
	}

/*
+----------------------------------------------+
|                  Getters                     |
+----------------------------------------------+
*/

	/*!
		Returns the last scene loaded. If no scene was loaded returns NULL.<BR>
		<BR>
		\sa OSceneLoader
	*/
	OSceneLoader* OSceneManager::lastOSceneLoaded_Ptr()
	{
		return _lastOSceneLoaded;
	}
	/*!
		Returns the scene with the name \a sceneName. If scene isn't found returns 0.<BR>
		<BR>
		\sa OSceneLoader
	*/
	OSceneLoader* OSceneManager::OScene_Ptr( QString sceneName )
	{
		for( int i = 0; i < _oSceneLoadedList.size() ; i++ )
		{
			if( _oSceneLoadedList.at(i)->osmSceneName() == sceneName )
			{
				return _oSceneLoadedList.at(i);
			}
		}

		return 0;
	}
}