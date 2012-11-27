#ifndef __CNOTI_OSCENE_LOADER_H_
#define __CNOTI_OSCENE_LOADER_H_

#include "Cnoti3d_global.h"
#include "oSceneLoader/include/OgreOSMScene.h"
// Qt
#include <QString>

class OSMScene;

namespace Cnoti3D
{
	class OSceneCallback;

	class CNOTI3DLIB_EXPORT OSceneLoader
	{
	public:

		OSceneLoader();
		OSceneLoader( Ogre::SceneManager *sceneMgr, Ogre::RenderWindow *window, const QString name, OSceneCallback *callBacks = NULL );
		~OSceneLoader();

		/** Process a OSM scene
		@remarks
		Process a OSM scene
		*/
		bool loadScene();

		//virtual LoadModel()
		//virtual LoadAnimation( std::string *animFilename ) = 0;

		const QString& osmSceneName();
		OSMScene::CameraList& getCameraList();
		OSMScene::EntityList& getEntityList();
		OSMScene::HelperList& getHelperList();
		OSMScene::LightList& getLightList();

	public:
		/// Create an oE_Loader Callback object to post-process created objects
		OSceneCallback* _callbacks;

	private:
		QString _osmSceneName;
		OSMScene* _OSMScene;	// Create the scene loader
	};
}
#endif	// __CNOTI_OSCENE_LOADER_H_
