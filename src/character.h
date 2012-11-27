#ifndef CHARACTER_H
#define CHARACTER_H

#include "Cnoti3d_global.h"
#include "Object3DAbstract.h"
// Qt
#include <QObject>
#include <QMultiHash>
#include <QQueue>
#include <QTime>
// Ogre
#include <OgreEntity.h>

class QPropertyAnimation;

namespace Cnoti3D {

	class CharacterMovement;

	class CNOTI3DLIB_EXPORT Character : public Object3DAbstract
	{
		Q_OBJECT
		Q_PROPERTY(QVector3D lookTo READ lookingTo WRITE setOrientation)
		Q_PROPERTY(bool animated READ isAnimated WRITE setAnimated)
//		Q_PROPERTY(qreal animationTime READ animationTime WRITE setAnimationTime)
	public:
		Character( const QString& name, Ogre::SceneManager* sceneManager, QString defaultAnimation, QObject *parent = 0 );
		virtual void reset();
		virtual void update(float timeSinceLastFrame);
		virtual bool isVisible()const;
		virtual void show();
		virtual void hide();

		virtual QString name()const;
		void addNode( Ogre::Node* node);
		void addNode( const QString& name);
		void addNodeList( const QStringList& nameLst);

		void moveTo( QVector3D destination );
		void moveTo(QList<QVector3D> destinationList);
		void addPointToMove(QVector3D destination);
		void rotateTo( QVector3D destination );

		CharacterMovement* movement_Ptr(){ return _movement; }
		Ogre::SceneNode* sceneNode(){return _node;}
		QVector3D position()const;

		void setPosition(const QVector3D& pos);
		void setPosition(qreal x, qreal y, qreal z);

		QVector3D lookingTo(){return _orientation;}

	public slots:
		void setOrientation(const QVector3D& lookTo);
		void setOrientation(qreal x, qreal y, qreal z);
		void startWalking();
		void stopWalking();

	protected:
		Ogre::SceneManager *sceneManager(){return _sceneManager;}

	private:
		QVector3D _orientation;
		bool _isVisible;
		Ogre::SceneNode* _node;
		Ogre::SceneManager* _sceneManager;
		CharacterMovement* _movement;


	/***************************/
	/*        ANIMATION        */
	/***************************/
	public:
		enum BlendingTransition {
		  BLEND_SWITCH,             // stop source and start dest
		  BLEND_WHILE_ANIMATING,    // cross fade, blend source animation out while blending destination animation in
		  BLEND_THEN_ANIMATE        // blend source to first frame of dest, when done, start dest anim
		};

		bool addAnimationEntity(QString entityName);
		void removeAnimationEntity(QString entityName);

		void startAnimation(QString animationName, bool loop = false);
		void startAnimation(QString animationName, qreal duration, bool loop = false);
		void queueAnimation(QString animationName, bool loop = false);
		void queueAnimation(QString animationName, qreal duration, bool loop);
		void stopAnimation();
		void stopCurrentAnimation();

		QString defaultAnimation(){return _defaultAnimation;}
		void setDefaultAnimation(QString name){_defaultAnimation = name;}
		void enableDefaultAnimation(){_defaultAnimationEnabled = true;}
		void disableDefaultAnimation(){_defaultAnimationEnabled = false;}
		bool isDefaultAnimationEnable() const {return _defaultAnimationEnabled;}

		bool isAnimated();
		void setAnimated(bool animate);

		bool hasAnimation(QString animation);

//		qreal animationTime();

	signals:
		void animationStarted(QString animationName);
		void animationFinished(QString animationName);

//	protected:
//		void setAnimationTime(qreal time);

	private:
		typedef struct animationInfo {
			QString name;							// Animation name
			bool loop;								// Is a loop animation
			qreal duration;							// Total duration
			qreal timeMultiplier;					// So it can have diferent durations for the same animation
			QList<Ogre::AnimationState *> list;		// Animations states list
		} AnimationInfo;

		QString _defaultAnimation;
		QString _currentAnimation;
//		QPropertyAnimation *_timeAnimation;
		int  _transition;
		bool _isAnimated;
		bool _isDoingTransition;
		bool _defaultAnimationEnabled;
		bool _dontEmitSignal;						// Dont send signal for an animation that already send the stopped signal (happens when _defaultAnimationEnabled is true)
		QMultiHash<QString, Ogre::Entity *> _animationsList;
		QQueue<AnimationInfo *> _animationQueue;
		// Functions
		void startDefaultAnimation();
		Ogre::Entity *ogreEntityFromName(QString entityName);
		void enableAnimationState(Ogre::AnimationState *animState);
		void disableAnimationState(Ogre::AnimationState *animState);
		void disableAnimation(AnimationInfo* animation);
		void enableAnimation(AnimationInfo* animation);
		void runAnimation(qreal duration = -1);
		void addTime(qreal timeSinceLastFrame);
		void updateBlendAnimation(qreal time);
		void updateAnimation(qreal time);
		void removeQueuedAnimations();

	private slots:
		void switchAnimation();
	};

}	//namespace Cnoti3D
#endif // CHARACTER_H
