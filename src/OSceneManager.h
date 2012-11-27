#ifndef __CNOTIOSCENEMANAGER_H
#define __CNOTIOSCENEMANAGER_H

#include "Cnoti3d_global.h"
#include "OgreSceneManager.h"
//Qt
#include <QList>

class QString;

namespace Cnoti3D
{
	class OSceneLoader;

	class CNOTI3DLIB_EXPORT OSceneManager
	{
		public:
			OSceneManager();
			~OSceneManager();

			bool loadSceneFromFile( QString sceneName, Ogre::SceneManager* sceneMgr, Ogre::RenderWindow* renderWindow );

			OSceneLoader* lastOSceneLoaded_Ptr();
			OSceneLoader* OScene_Ptr( QString sceneName );

		private:
			OSceneLoader*        _lastOSceneLoaded;	// Pointer to our current scene
			QList<OSceneLoader*> _oSceneLoadedList;	// A vector of loaded scenes

	};
}

#endif // __CNOTIOSCENEMANAGER_H
