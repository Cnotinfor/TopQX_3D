/*!
\class LittleMozartBody littlemozartbody.h
This class implements the visual part of little mozart.

\sa LittleMozart
*/
#include "Character.h"
#include "CharacterMovement.h"
#include "UtilFunctions.h"
// Qt
#include <QStringList>
#include <QPropertyAnimation>
#include <QDebug>

namespace Cnoti3D {

Character::Character(const QString& name, Ogre::SceneManager* sceneManager, QString defaultAnimation, QObject *parent ):
	Cnoti3D::Object3DAbstract(parent),
	// Animation
	_transition(BLEND_SWITCH),
	_isDoingTransition(false),
	_defaultAnimationEnabled(true),
	_defaultAnimation(defaultAnimation),
	_dontEmitSignal(false)
{
	_sceneManager = sceneManager;
	_isVisible = true;
	_node = sceneManager->getRootSceneNode()->createChildSceneNode( name.toStdString() );
	_movement = new CharacterMovement( _node );
	QObject::connect(_movement, SIGNAL(startedMoving()), (Object3DAbstract*)this, SLOT(startWalking()));
	QObject::connect(_movement, SIGNAL(stopedMoving()), (Object3DAbstract*)this, SLOT(stopWalking()));
}

void Character::reset()
{
	blockSignals(true);
	removeQueuedAnimations();
	switchAnimation();
	_movement->stopMoving();
	blockSignals(false);
}

void Character::update(float timeSinceLastFrame)
{
	//qDebug() << "[Character::update] - TimeSinceLastFrame:" << timeSinceLastFrame ;
	// Movement
	_movement->update( timeSinceLastFrame );
	// Animation
	addTime(timeSinceLastFrame);
}

bool Character::isVisible()const
{
	return _isVisible;
}

void Character::show()
{
	_isVisible = true;
}

void Character::hide()
{
	_isVisible = false;
}

void Character::setOrientation(const QVector3D& lookTo)
{
	// TODO: remove _orientation variable, use values direct from the node
	_orientation = lookTo;
	_movement->setLookTo(lookTo);
	rotateTo(lookTo);
}

void Character::setOrientation(qreal x, qreal y, qreal z)
{
	setOrientation(QVector3D(x, y, z));
}

QString Character::name()const
{
	return QString( _node->getName().c_str() );
}

void Character::addNode(Ogre::Node* node)
{
	qDebug() << "[Character::addNode] - Name: " << node->getName().c_str();
	// Removes from is parent sceneNode
	node->getParent()->removeChild(node->getName());
	// Adds to the character sceneNode
	_node->addChild(node);
	// Add entity to animation
	addAnimationEntity(node->getName().c_str());
}

void Character::addNode(const QString& name)
{
	Ogre::Node* node;
	if( _sceneManager->hasSceneNode( name.toStdString() ) )
	{
		node = _sceneManager->getSceneNode( name.toStdString() );
		addNode( node);
	}
	else
	{
		qWarning()<< "[Character::addNode]:The node (" + name + ") doesn't exist.";
	}
}

void Character::addNodeList(const QStringList& nameLst)
{
	for(int i=0;i<nameLst.size();i++)
	{
		addNode( nameLst.at(i));
	}
}

void Character::addPointToMove( QVector3D destination )
{
	_movement->addPointToMove( destination );
}

void Character::moveTo( QVector3D destination )
{
	_movement->moveTo( destination );
}

void Character::moveTo(QList<QVector3D> destinationList)
{
	_movement->moveTo( destinationList );
}

void Character::rotateTo( QVector3D destination )
{
	_movement->rotateTo(destination);
}

void Character::startWalking()
{
	startAnimation( "Walk", true );
}

void Character::stopWalking()
{
	qDebug() << "[Character::stopWalking]";
	stopCurrentAnimation();
}

/***************************************/
/*             SETTERS                 */
/***************************************/
QVector3D Character::position()const
{
	return UtilFunctions::ogreVector3ToQVector3d(_node->getPosition());
}

/***************************************/
/*             SETTERS                 */
/***************************************/

void Character::setPosition(const QVector3D& pos)
{
	//qDebug() << "[Character::setPosition]" << pos ;
	Object3DAbstract::setPosition( pos );
	_node->setPosition( pos.x(), pos.y(), pos.z() );
}

void Character::setPosition(qreal x, qreal y, qreal z)
{
	this->setPosition(QVector3D(x, y, z));
}


/***************************/
/*        ANIMATION        */
/***************************/

void Character::switchAnimation()
{
	_isAnimated = false;
//	qDebug() << "[Character::switchAnimation] Character:" << name();
	if(_animationQueue.isEmpty())
	{
		qWarning() << "[Character::switchAnimation] - Queue empty starting default animation";
		startDefaultAnimation();
		return;
	}
	// if previously was doing a transition
	if(_isDoingTransition)
	{
//		qDebug() << "[Character::switchAnimation] Finishing transition between animations";
		// Removes and disables the animation at the start of the list
		AnimationInfo* lastAnimation = _animationQueue.dequeue();
		disableAnimation(lastAnimation);
		// Enables the new animation
		enableAnimation(_animationQueue.first());
		runAnimation();
		_isDoingTransition = false;
	}
	else
	{
		QString animationName = _animationQueue.first()->name;
//		qDebug() << "[Character::switchAnimation] Emit animationFinished" << animationName;
		if(animationName != _defaultAnimation && !_dontEmitSignal)
		{
			emit animationFinished(animationName);
		}
		if(_animationQueue.size() == 1)
		{
			if(_defaultAnimationEnabled)
			{
//				qDebug() << "[Character::switchAnimation] Queued default animation";
				queueAnimation(_defaultAnimation, true);
			}
			else
			{
				_dontEmitSignal = true;
//				qDebug() << "[Character::switchAnimation] Default animation disabled, no animation will be queued";
				return;
			}
		}

		if(_dontEmitSignal)
		{
			_dontEmitSignal = false;
		}
		// Switch animation
		if( _transition == BLEND_SWITCH )
		{
//			qDebug() << "[Character::switchAnimation] Character:" << name() << "Transition: BLEND_SWITCH";
			// Removes the first animation and disables it
			disableAnimation(_animationQueue.dequeue());
			// Enables the new animation
			enableAnimation(_animationQueue.first());
			// Start animmation (timer)
			runAnimation();
		}
		else
		{
			// start the transition
			_isDoingTransition = true;
			_isAnimated = true;
//			_timeAnimation->setStartValue(0.0f);
//			_timeAnimation->setEndValue(1.0f);	//Transition time.
//			_timeAnimation->setDuration(1.0f * 1000);
//			_timeAnimation->setLoopCount(1);
//			_timeAnimation->start();
		}
	}
}

void Character::updateAnimation(qreal time)
{
//	qDebug() << "[Character::updateAnimation] Character:" << name() << "Time: " << time;
	if(!_animationQueue.isEmpty())
	{
		time = time / _animationQueue.first()->timeMultiplier;
		Ogre::AnimationState *animationState;
		QListIterator<Ogre::AnimationState *> it(_animationQueue.first()->list);
		while(it.hasNext())
		{
			animationState = it.next();
//			animationState->setTimePosition(time);
			animationState->addTime(time);
		}

//qDebug() << "[Character::updateAnimation] Character:" << name() << "Animation: " << _animationQueue.first()->name;
//qDebug() << "[Character::updateAnimation] Character:" << name() << "Time: " << time << animationState->getTimePosition();


		// Check if at the end of the animation
		if(animationState->hasEnded())
		{
			switchAnimation();
		}
	}
}

/*!
  Blends two animations.
*/
void Character::updateBlendAnimation(qreal time)
{
	// still blending, change weights
	Ogre::AnimationState *animationState;
//	qreal weightOldAnimation = _timeAnimation->currentTime() / _timeAnimation->totalDuration();
	qreal weightOldAnimation = _animationQueue.first()->list.at(0)->getTimePosition() / _animationQueue.first()->duration;
	// Update source animation
	QListIterator<Ogre::AnimationState *> it(_animationQueue.first()->list);
	while(it.hasNext())
	{
		animationState = it.next();
		animationState->setWeight(weightOldAnimation);
	}
	// Update target animation
	QListIterator<Ogre::AnimationState *> it_1(_animationQueue[1]->list);
	while(it_1.hasNext())
	{
		animationState = it_1.next();
		animationState->setWeight(1.0 - weightOldAnimation);
		if(_transition == BLEND_WHILE_ANIMATING)
		{
//			animationState->setTimePosition(time);
			animationState->addTime(time);
		}
	}
	// Check if transition has ended (wheight of new animations is 1)
	if(animationState->getWeight() == 1)
	{
		switchAnimation();
	}
}

/*!
  Adds an entity to be animated.
*/
bool Character::addAnimationEntity(QString entityName)
{
	qDebug() << "[Character::addAnimationEntity] Character:" << name();
	Ogre::Entity *entity = ogreEntityFromName(entityName);
	if(entity == NULL)
	{
		return false;
	}
	// Check if entity alredy in list
	if(_animationsList.values(entityName).isEmpty())
	{
		// Gets all animations from this entity
		Ogre::AnimationStateSet* animStateSet = entity->getAllAnimationStates();
		if(animStateSet == 0)
		{
			qWarning() << "[Character::addAnimationEntity] - Entity not animated:" << entityName;
			return false;
		}
		Ogre::AnimationStateIterator it = animStateSet->getAnimationStateIterator();
		Ogre::AnimationState *mAnimState;
		while(it.hasMoreElements())
		{
			mAnimState = it.getNext();
			QString animationName = mAnimState->getAnimationName().c_str();
			_animationsList.insert(animationName, entity);
		}
	}
	else
	{
		qWarning() << "[Character::addAnimationEntity] Entity already exists";
		return false;
	}
	return true;
}

/*!
  Removes an entity.
*/
void Character::removeAnimationEntity(QString entityName)
{
	qDebug() << "[Character::removeAnimationEntity]";
	Ogre::Entity *entity = ogreEntityFromName(entityName);
	if(entity == NULL)
	{
		return;
	}
	QMutableHashIterator<QString, Ogre::Entity *> it(_animationsList);
	while(it.hasNext())
	{
		it.next();
		if(it.value() == entity)
		{
			it.remove();
		}
	}
}

/*!
  Starts an animation
*/
void Character::startAnimation(QString animationName, bool loop)
{
//	qDebug() << "[Character::startAnimation] Character:" << name() << "Animation:" << animationName << "Loop:" << loop;
	if(_animationsList.contains(animationName))
	{
		Ogre::AnimationState *animState = _animationsList.value(animationName)->getAnimationState(animationName.toStdString());
		startAnimation(animationName, animState->getLength(), loop);
	}
	else
	{
		qWarning() << "[Character::startAnimation] Character:" << name() << "Animation not found.";
	}
}

/*!
  Starts an animation giving the aniimation duration.
*/
void Character::startAnimation(QString animationName, qreal duration, bool loop)
{
	qDebug() << "[Character::startAnimation] Character:" << name() << "Animation:" << animationName << "Duration:" << duration << "Loop:" << loop;
	if(_animationsList.contains(animationName))
	{
		Ogre::AnimationState *animState;

		// Remove all queued animations
		removeQueuedAnimations();

		// Gets the animations states for this animation and add the list in the queue
		QList<Ogre::AnimationState *> list;
		QListIterator<Ogre::Entity *> it(_animationsList.values(animationName));
		while(it.hasNext())
		{
			animState = it.next()->getAnimationState(animationName.toStdString());
			animState->setLoop(loop);
			//enableAnimationState(animState);
			list << animState;
		}
		AnimationInfo* animInfo = new AnimationInfo;
		animInfo->name = animationName;
		animInfo->loop = loop;
		animInfo->duration = duration;
		animInfo->timeMultiplier = duration / animState->getLength();
		animInfo->list = list;
		_animationQueue.enqueue(animInfo);
		stopCurrentAnimation();
	}
	else
	{
		qWarning() << "[Character::startAnimation] Character:" << name() << "Animation not found.";
	}
}

/*!
  Queues an animation.
*/
void Character::queueAnimation(QString animationName, bool loop)
{
	if(_animationsList.contains(animationName))
	{
//		qDebug() << "[Character::queueAnimation] Character:" << name() << "Animation:" << animationName << "Loop:" << loop;
		Ogre::AnimationState *animState = _animationsList.value(animationName)->getAnimationState(animationName.toStdString());
		queueAnimation(animationName, animState->getLength(), loop);
	}
	else
	{
		qWarning() << "[Character::queueAnimation] Character:" << name() << "Animation not found.";
	}
}

/*!
  Queues an animation.
*/
void Character::queueAnimation(QString animationName, qreal duration, bool loop)
{
	if(_animationsList.contains(animationName))
	{
		qDebug() << "[Character::queueAnimation] Character:" << name() << "Animation:" << animationName << "Duration:" << duration << "Loop:" << loop;
		if(_currentAnimation == _defaultAnimation && _animationQueue.size() == 1 && animationName != _defaultAnimation)
		{
			startAnimation(animationName, duration, loop);
			return;
		}
		Ogre::AnimationState *animState;
		QList<Ogre::AnimationState *> list;
		QListIterator<Ogre::Entity *> it(_animationsList.values(animationName));
		while(it.hasNext())
		{
			animState = it.next()->getAnimationState(animationName.toStdString());
			animState->setLoop(loop);
			list << animState;
		}
		AnimationInfo* animInfo = new AnimationInfo;
		animInfo->name = animationName;
		animInfo->loop = loop;
		animInfo->duration = duration;
		animInfo->timeMultiplier = duration / animState->getLength();
		animInfo->list = list;
//		qDebug() << "[Character::startAnimation] Character:" << name() << "Time x:" << animInfo->timeMultiplier << duration << animState->getLength();
		_animationQueue.enqueue(animInfo);
//		qDebug() << "[Character::queueAnimation] Animation Queded";
	}
	else
	{
		qWarning() << "[Character::queueAnimation(QString,qreal,bool)] Character:" << name() << "Animation not found.";
	}
}

/*!
  Stops all animation.
  Removes all animations from list and stops the currrent animation.
*/
void Character::stopAnimation()
{
	qDebug() << "[Character::stopAnimation] Character:" << name();
	// Remove all queued animations
	removeQueuedAnimations();
	// Stops the current animation
	stopCurrentAnimation();
}

/*!
  Set the current animation at the end of its animation and removes the loop option.
*/
void Character::stopCurrentAnimation()
{
	qDebug() << "[Character::stopCurrentAnimation] Character:" << name();
	_isAnimated = false;
	if(_animationQueue.size() == 2)
	{
		if(_currentAnimation == _defaultAnimation && _animationQueue.at(1)->name == _defaultAnimation )
		{
			_isAnimated = true;
			return;
		}
	}
	// Stop current animation
	Ogre::AnimationState *state;
	QListIterator<Ogre::AnimationState *> it(_animationQueue.first()->list);
	while(it.hasNext())
	{
		state = it.next();
		state->setLoop(false);
		state->setTimePosition(state->getLength());
	}
//	qDebug() << "[Character::stopCurrentAnimation] Character:" << name() << state->getLoop() << state->getTimePosition();
	_isAnimated = true;
}

/*!
	Starts the default aniimations
*/
void Character::startDefaultAnimation()
{
	if(!_defaultAnimationEnabled)
	{
		qDebug() << "[Character::startDefaultAnimation] Character:" << name() << "Default animation not enabled";
		return;
	}
//	qDebug() << "[Character::startDefaultAnimation] Character:" << name();
	Ogre::AnimationState *animState;
	QList<Ogre::AnimationState *> list;
	QListIterator<Ogre::Entity *> it(_animationsList.values(_defaultAnimation));
	while(it.hasNext())
	{
		animState = it.next()->getAnimationState(_defaultAnimation.toStdString());
		animState->setLoop(true);
		enableAnimationState(animState);
		list << animState;
	}
	AnimationInfo* animInfo = new AnimationInfo;
	animInfo->name = _defaultAnimation;
	animInfo->loop = true;
	animInfo->duration = list.first()->getLength();
	animInfo->timeMultiplier = 1;
	animInfo->list = list;
	_animationQueue.enqueue(animInfo);

//	_timeAnimation->setStartValue(0.0f);
////qDebug() << "[Character::startIdleAnimation]" << animState->getLength();
//	_timeAnimation->setEndValue(animInfo->duration);
//	_timeAnimation->setDuration(animInfo->duration * 1000);
//	_timeAnimation->setLoopCount(-1);
//	_timeAnimation->start();
	_currentAnimation = _defaultAnimation;
	_isAnimated = true;
}

/*!
  Gets the entity ponter from the entity name.
*/
Ogre::Entity *Character::ogreEntityFromName(QString entityName)
{
	qDebug() << "[Character::ogreEntityFromName] Entity name:" << entityName;
	// Gets Ogre Scene Manager
	Ogre::SceneManagerEnumerator::SceneManagerIterator it = Ogre::Root::getSingletonPtr()->getSceneManagerIterator();
	if(it.begin() == it.end())
	{
		qWarning() << "[Character::addEntity] Couldn't obtain ogre scene manager";
		return NULL;
	}
	Ogre::SceneManager* sceneMgr = it.getNext();
	Ogre::String ogreStr = entityName.toStdString();
	if(!sceneMgr->hasEntity(ogreStr))
	{
		qWarning() << "[Character::addEntity] Entity not found:" << entityName;
		return NULL;
	}
	return sceneMgr->getEntity(ogreStr);
}

/*!
  Enables as animation state.
*/
void Character::enableAnimationState(Ogre::AnimationState *animState)
{
//	qDebug() << "[Character::enableAnimationState] Character:" << name() << "Animation:" << animState->getAnimationName().c_str();
	animState->setTimePosition(0);
	animState->setWeight(1);
	animState->setEnabled(true);
}

/*!
  Disables as animation state.
*/
void Character::disableAnimationState(Ogre::AnimationState *animState)
{
//	qDebug() << "[Character::disableAnimationState] Character:" << name() << "Animation:" << animState->getAnimationName().c_str();
	animState->setLoop(false);
	animState->setTimePosition(0);
	animState->setWeight(0);
	animState->setEnabled(false);
}

/*!
  Disables an animation (a list of animation states).
*/
void Character::enableAnimation(AnimationInfo* animation)
{
//	qDebug() << "[Character::enableAnimation] Character:" << name() << "Animation:" << animation->name;
	QListIterator<Ogre::AnimationState *> it(animation->list);
	while(it.hasNext())
	{
		Ogre::AnimationState *nextAnimation = it.next();
		enableAnimationState(nextAnimation);
	}
}

/*!
  Disables an animation (a list of animation states).
  Clear list an deletes animation info,
*/
void Character::disableAnimation(AnimationInfo* animation)
{
//	qDebug() << "[Character::disableAnimation] Character:" << name() << "Animation:" << animation->name;
	QListIterator<Ogre::AnimationState *> it(animation->list);
	while(it.hasNext())
	{
		Ogre::AnimationState *nextAnimation = it.next();
		disableAnimationState(nextAnimation);
	}
	animation->list.clear();
	delete(animation);
}

/*!

*/
void Character::runAnimation(qreal duration)
{
	QString animationName = _animationQueue.first()->name;
	qDebug() << "[Character::runAnimation]  Character:" << name() << "Duration:" << duration << "Name:" << animationName;
	// Check duration
//	if(duration == -1)
//	{
//		duration = _animationQueue.first()->duration;
//	}
	// Loop
//qWarning() << "[Character::runAnimation] Name:" << _animationQueue.first().at(0)->getAnimationName().c_str() << "loop" << _animationQueue.first().at(0)->getLoop();
//	if(_animationQueue.first()->loop)
//	{
//		_timeAnimation->setLoopCount(-1);	// loop true;
//	}
//	else
//	{
//		_timeAnimation->setLoopCount(1); // run once;
//	}
//	_timeAnimation->setStartValue(0.0f);
//	_timeAnimation->setEndValue(_animationQueue.first()->list.at(0)->getLength());
//	_timeAnimation->setDuration(duration * 1000);
//	_timeAnimation->start();
	qDebug() << "[Character::runAnimation]  Character:" << name() << "Emit animationStarted" << animationName;
	emit animationStarted(animationName);
	_currentAnimation = animationName;
	_isAnimated = true;
}

/*!

*/
void Character::removeQueuedAnimations()
{
	qDebug() << "[Character::removeQueuedAnimations]";
	// Removes all queued animations
	for(int i = _animationQueue.size() - 1; i > 0 ; i--)
	{
		QString animationStr = _animationQueue.at(i)->name;
//		qDebug() << "[Character::removeQueuedAnimations]" << animationStr;
		qDebug() << "[Character::removeQueuedAnimations] Character:" << name() << "Emit animationStarted" << animationStr;
		emit animationStarted(animationStr);
		qDebug() << "[Character::removeQueuedAnimations] Character:" << name() << "Emit animationFinished" << animationStr;
		emit animationFinished(animationStr);
		// Remove
		delete(_animationQueue.at(i));
		_animationQueue.removeAt(i);
	}
}

bool Character::isAnimated()
{
	return _isAnimated;
}

void Character::setAnimated(bool animate)
{

	qDebug() << "[Character::setAnimated]";
	if(animate)
	{
		startDefaultAnimation();
	}
	_isAnimated = animate;
}

void Character::addTime(qreal timeSinceLastFrame)
{
	if(!_isAnimated)
	{
		return;
	}
//	qDebug() << "[Character::addTime]Character:" << name() << "Time:" << timeSinceLastFrame;
	// Add time
	if(_isDoingTransition)
	{
		updateBlendAnimation(timeSinceLastFrame);
	}
	else
	{
		updateAnimation(timeSinceLastFrame);
	}
}

bool Character::hasAnimation(QString animation)
{
	return _animationsList.contains(animation);
}

} //namespace Cnoti3D
