#ifndef CNOTI3DOBJECTMANAGER_H
#define CNOTI3DOBJECTMANAGER_H

#include <QHash>
#include <QString>
#include <QVector>

namespace Cnoti3D {

	class Object3DAbstract;
	class Object3D;
	class Object3DAnimated;
	class Object3DMousable;

	class ObjectManager
	{
		public:
			ObjectManager();
			~ObjectManager();

			bool isCnoti3dObject( QString name );
			Object3DMousable* isObjectMouseAwareAvailable( QString name );
			Object3DAbstract* object3DAbstract( QString name );
			bool addObject3DAbstract( Object3DAbstract* object );
			void removeCnoti3dObject( QString name );
			void updateObjects(qreal timeSinceLastFrame);
			bool isAnimated( Object3DAbstract* object );

		private:
			QHash<QString, Object3DAbstract*> _objectHash;
			QVector<Object3DAbstract*> _obj3DAnimLst;
	};

}	// namespace Cnoti3D
#endif // CNOTI3DOBJECTMANAGER_H
