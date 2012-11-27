#ifndef OBJECT3DANIMATED_H
#define OBJECT3DANIMATED_H

#include "Cnoti3d_global.h"
#include "Object3d.h"

#include <Ogre.h>
#include <QQueue>

namespace Cnoti3D
{
	class CNOTI3DLIB_EXPORT Object3DAnimated : public Object3D
	{
	Q_OBJECT
	public:
		enum BlendingTransition
		  {
			 BlendSwitch,         // stop source and start dest
			 BlendWhileAnimating,   // cross fade, blend source animation out while blending destination animation in
			 BlendThenAnimate      // blend source to first frame of dest, when done, start dest anim
		  };

		struct Transition {
		  BlendingTransition transition;	// Transition type
		  float duration;					// Transition duration
		  float timeleft;					// Transition time to end
		  Ogre::AnimationState* target;		// New animation
		  bool loop;						// Keep animation in loop
		  bool force;						// Force the transition
		} ;

		Object3DAnimated( Ogre::SceneManager* sceneManager,
						  const QString& objectName );
		Object3DAnimated( Ogre::Entity* entity );
		void blend( const QString& animation,
					BlendingTransition transition,
					int duration,
					bool loop = false,
					bool force = false );

		Ogre::AnimationState* _mAnimationState;
		QQueue<Transition> _transitionsLst;

        void startAnimation( QString animation, bool loop = false );
        void stopAnimation();

		void setDefaultAnimation( QString defaultAnimation );

	public slots:
		virtual void update ( int time );

	protected:
		QString _defaultAnimation;

	private:
		bool _loop;
		void init();
	};
}
#endif // OBJECT3DANIMATED_H
