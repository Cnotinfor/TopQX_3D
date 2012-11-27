#if !defined(_CNOTI3D_H)
#define _CNOTI3D_H

#include "Cnoti3d_global.h"
#include <QString>
// Qt

namespace Cnoti3D
{

	class QOgreGraphicsScene;
	class QOgreGraphicsView;

	class Object3DAbstract;
	class ObjectManager;
	class StateManager;

	class CNOTI3DLIB_EXPORT Cnoti3d
	{
	public:
		enum Clickable{clickable,notClickable};
		Cnoti3d();
		~Cnoti3d();
		virtual void shutdown() = 0;

		virtual void resize( int width, int height ) = 0;

		virtual void loadScene(const QString& filename) = 0;
		virtual Object3DAbstract* createObject(QString objectName, QString meshFile, QString animationFile = "", Clickable clickable = notClickable) = 0;
		virtual Object3DAbstract* createObject(QString objectName, QString meshFile, QString animationFile, qreal x, qreal y, qreal z, Clickable clickable = notClickable) = 0;
		virtual Object3DAbstract* createObject(QString objectName, QString meshFile, qreal x, qreal y, qreal z, Clickable clickable = notClickable) = 0;
//		virtual void loadResource(QString resourceFile) = 0;
//		virtual void unloadResource(QString resourceFile) = 0;

		virtual Object3DAbstract* object(const QString& objectName) = 0;
	protected:
		ObjectManager* _3dObjectsMgr;

	};

}

#endif  //_CNOTI3DGRAPHICSENGINE_H
