#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "Cnoti3d_global.h"
#include "Camera.h"
#include "Singleton.h"
// Qt
#include <QObject>
#include <QHash>
#include <QString>
// Ogre
#include <Ogre.h>

namespace Cnoti3D
{
	class CNOTI3DLIB_EXPORT CameraManager : public Singleton<CameraManager>
	{
		friend class Singleton<CameraManager>;
	public:
		Camera* addCamera(Ogre::SceneManager* sceneManager, const QString& name );
		Camera* camera( const QString& name )const;

		Camera* activeCamera()const;
		Camera* setActiveCamera( const QString& name,Ogre::Viewport* vp);

		void updateAllCamerasAspectRatio( qreal width, qreal height );

	protected:
		CameraManager();

	private:
		QHash <QString,Camera*> _cameraLst;
		Camera* _activeCamera;

	};
}
#endif // CAMERAMANAGER_H
