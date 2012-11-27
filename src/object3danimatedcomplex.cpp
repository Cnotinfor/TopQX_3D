/*!
\class Object3DAnimatedComplex object3danimatedcomplex.h <Object3DAnimatedComplex>

Implements a Object3DAnimated which have sequences of animations.

\sa Object3DAnimated
*/
#include "object3danimatedcomplex.h"

namespace Cnoti3D
{
    /*!
    Constructs a new Object3D.
    \param sceneManager     Ogre's SceneManager
    \param objectName       Object3D's name
    */
    Object3DAnimatedComplex::Object3DAnimatedComplex( Ogre::SceneManager* sceneManager,
                                                      const QString& objectName,
                                                      const QStringList& animationLst):
    Object3DAnimated( sceneManager, objectName )
    {
        _animationLst = animationLst;
    }

    /*!
    Constructs a new Object3D.
    \param entity   Ogre's entity
    */
    Object3DAnimatedComplex::Object3DAnimatedComplex( Ogre::Entity* entity,
                                                      const QStringList& animationLst):
    Object3DAnimated(entity)
    {
        _animationLst = animationLst;
    }

    /*!
    Updates the animation
    */
    void Object3DAnimatedComplex::update ( int time )
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
                    _mAnimationState->setEnabled(false);
                    _mAnimationState->setWeight(0);
                }
                _mAnimationState = _transitionsLst.first().target;
                _mAnimationState->setEnabled(true);
                _mAnimationState->setWeight(1);
                _mAnimationState->setLoop( _transitionsLst.first().loop );

                _transitionsLst.pop_front();
            }
            else
            {
                if(_transitionsLst.first().force)
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
                else if(_mAnimationState!=NULL)
                {
                    if(_mAnimationState->getTimePosition() >= _mAnimationState->getLength())
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
                }else
                {
                    ratio = _transitionsLst.first().timeleft / _transitionsLst.first().duration;
                    _transitionsLst.first().target->setWeight(1.0 - ratio);

                    if(_transitionsLst.first().transition == BlendWhileAnimating)
                    {
                        _transitionsLst.first().target->addTime(time);
                    }
                }
            }
        }
        if( _mAnimationState != 0 )
        {
            _mAnimationState->addTime(time);
        }
    }

    /*!
    Starts the animation sequence
    */
    void Object3DAnimatedComplex::startAnimation()
    {
        blend( _animationLst.at(0),BlendThenAnimate, 10, false );
        blend( _animationLst.at(1),BlendWhileAnimating, 10, true );
    }

    /*!
    Stopps the animation sequence
    */
    void Object3DAnimatedComplex::stopAnimation()
    {
        blend( _animationLst.at(2),BlendWhileAnimating, 10, false );
    }

}
