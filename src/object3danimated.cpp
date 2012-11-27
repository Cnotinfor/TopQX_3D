/*!
\class Object3DAnimated Object3dAnimated.h <Object3DAnimated>

Implements a Object3D which can be animated.

\sa Object3D
*/
#include "Object3dAnimated.h"

#include <QDebug>

namespace Cnoti3D
{
    /*!
    Constructs a new Object3D.
    \param sceneManager     Ogre's SceneManager
    \param objectName       Object3D's name
    */
    Object3DAnimated::Object3DAnimated( Ogre::SceneManager* sceneManager,
                                        const QString& objectName ):
    Object3D( sceneManager, objectName )
    {
        init();
    }

    /*!
    Constructs a new Object3D.
    \param entity   Ogre's entity
    */
    Object3DAnimated::Object3DAnimated( Ogre::Entity* entity ):
            Object3D(entity)
    {
        init();
    }

    /*!
    Initialises
    */
    void Object3DAnimated::init()
    {
        Ogre::AnimationStateIterator it = _entity->getAllAnimationStates()->getAnimationStateIterator();
        Ogre::AnimationState *animState;
        while(it.hasMoreElements())
        {
            animState = it.getNext();
            animState->setEnabled(false);
            animState->setWeight(0);
            animState->setTimePosition(0);
        }
        _mAnimationState  = NULL;
    }

    /*!
    Updates the animation
    */
    void Object3DAnimated::update ( int time )
    {
        float ratio;
        time/=25;        

        if( _transitionsLst.size() > 0 )
        {
            _transitionsLst.first().timeleft -= time;
            if( _transitionsLst.first().timeleft <= 0 )
            {
                // finish blending
                if(_mAnimationState!=NULL)
                {
					// Stops last active animation
                    _mAnimationState->setEnabled(false);
                    _mAnimationState->setWeight(0);
                }
				// Saves the new information of the animation
                _mAnimationState = _transitionsLst.first().target;
                _mAnimationState->setEnabled(true);
                _mAnimationState->setWeight(1);
                _mAnimationState->setLoop( _transitionsLst.first().loop );

                _transitionsLst.pop_front();
            }
            else
            {
                // still blending, advance weights
                ratio = _transitionsLst.first().timeleft / _transitionsLst.first().duration;
                if(_mAnimationState!=NULL)
                {
                    _mAnimationState->setWeight(ratio);
                }
                _transitionsLst.first().target->setWeight(1.0 - ratio);

                if(_transitionsLst.first().transition == BlendWhileAnimating)
                {
                    _transitionsLst.first().target->addTime(time);
                }
            }
        }
        if( _mAnimationState != 0 )
        {
            _mAnimationState->addTime(time);
        }
    }

    /*!
To blend is to fade between two states. In the animation process it allows to create a transition between two animations. With this sample code, you can blend animations in 3 ways:

- BlendSwitch: just switch to the second/destination animation (stop the first/source and start the second).
- BlendWhileAnimating: cross-fade, blend source animation out while blending destination animation in.
- BlendThenAnimate: pick source's current frame and blend it with destination's first frame.

\sa http://www.ogre3d.org/tikiwiki/AnimationBlender
    */
    void Object3DAnimated::blend( const QString& animation, BlendingTransition transition, int duration, bool loop, bool force)
	{
		Ogre::AnimationState* newAnimationState;
		try
		{
			newAnimationState = _entity->getAnimationState( animation.toStdString() );
		}
		catch( Ogre::ItemIdentityException )
		{
			qDebug() << QString( "[Object3DAnimated::blend]: Animation " + animation + " not found");
			return;
		}

		if( _mAnimationState == newAnimationState )
		{
			return;
		}

        if( transition == BlendSwitch )
        {
            if( _mAnimationState != NULL )
            {
                _mAnimationState->setEnabled(false);
            }
            _mAnimationState = _entity->getAnimationState( animation.toStdString() );
            _mAnimationState->setEnabled(true);
            _mAnimationState->setWeight(1);
            _mAnimationState->setTimePosition(0);
            _mAnimationState->setLoop(loop);
        }
        else
        {
            _transitionsLst.push_back(Transition());
            _transitionsLst.last().loop = loop;
            _transitionsLst.last().transition = transition;
            _transitionsLst.last().force = force;
            _transitionsLst.last().timeleft =  _transitionsLst.last().duration = duration;
            _transitionsLst.last().target = _entity->getAnimationState( animation.toStdString() );
            _transitionsLst.last().target->setEnabled(true);
            _transitionsLst.last().target->setWeight(0);
            _transitionsLst.last().target->setTimePosition(0);
        }
    }

	void Object3DAnimated::startAnimation( QString animation, bool loop )
	{
		blend( animation, BlendWhileAnimating, 10, loop );
	}

	void Object3DAnimated::stopAnimation()
	{
		blend( _defaultAnimation, BlendWhileAnimating, 0.2 );
	}

/* Setters */
	void Object3DAnimated::setDefaultAnimation( QString defaultAnimation )
	{
		_defaultAnimation = defaultAnimation;
		blend( _defaultAnimation, BlendThenAnimate, 0.2 );
	}
}

