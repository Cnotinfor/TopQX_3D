#if !defined(_OBJECT3D_H)
#define _OBJECT3D_H

#include "Cnoti3d_global.h"
#include "Object3dAbstract.h"
// Ogre
#include <Ogre.h>

namespace Cnoti3D
{
	//
	// Our ogre object class
	//
	class CNOTI3DLIB_EXPORT Object3D : public Object3DAbstract
	{
	public:
		Object3D( Ogre::SceneManager* sceneManager, const QString& objectName );
		Object3D( Ogre::Entity* entity );
		~Object3D();

		virtual void update(float timeSinceLastFrame);
		void show();
		void hide();

		QString name()const;
		QVector3D position()const;
		bool isVisible()const;
		qreal xPos()const;
		qreal yPos()const;
		qreal zPos()const;
		Ogre::SceneNode* sceneNode()const;
		Ogre::Node* node()const;
		Ogre::Entity* entity()const;

		//void setNode(Ogre::Node* node);
		void setSceneNode(Ogre::SceneNode* sceneNode);
		void setPosition(qreal x, qreal y, qreal z);
		void setPosition(const QVector3D& pos);
		void setOrientation(const QVector3D& lookTo);

	protected:
		Ogre::Entity* _entity;
		Ogre::SceneManager* _sceneManager;
	};

}

#endif  //_OBJECT3D_H
