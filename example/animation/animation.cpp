#include "animation.h"
#include "AnimationSelection.h"
// Cnoti3D
#include <OgreWidget>
// Ogre

// Qt
#include <QDebug>
#include <QKeyEvent>

AnimationExample::AnimationExample(QObject* parent)
: Cnoti3D::QtOgre(parent)
{
    qDebug()<<"Debug";
    qWarning()<<"Warning";
	qCritical()<<"Critical";

	initialise();

	Ogre::Light* pointLight = ogreWidget()->ogreSceneManager()->createLight("pointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setPosition(Ogre::Vector3(0, 50, 500));

	pointLight->setDiffuseColour(1.0, 1.0, 1.0);
	pointLight->setSpecularColour(1.0, 1.0, 1.0);

	loadScene("mozart.osm");

	_character = new Cnoti3D::Character("Mozart", ogreWidget()->ogreSceneManager(), "Idle");

	_character->addNode("Mozart_Body");
	_character->addNode("Mozart_Head02");
	_character->setPosition( 0, -100, 0 );

	addObject(_character);

	QStringList animationList;
	Ogre::AnimationStateIterator it = ogreWidget()->ogreSceneManager()->getEntity("Mozart_Head02")->getAllAnimationStates()->getAnimationStateIterator();
	Ogre::AnimationState *mAnimState;
	while(it.hasMoreElements())
	{
		mAnimState = it.getNext();
		animationList << QString(mAnimState->getAnimationName().c_str());
//		qDebug() << QString(mAnimState->getAnimationName().c_str());
//		qDebug() << QString::number(mAnimState->getLength());
	}

	ogreWidget()->show();

	_animationSelector = new AnimationSelection(_character, animationList);
	_animationSelector->show();

	_character->setAnimated(true);
	_character->startAnimation("Idle", true);

	connect(ogreWidget(), SIGNAL(keyPressedEvent(QKeyEvent*)), this, SLOT(keyPressEvent(QKeyEvent*)));
}

AnimationExample::~AnimationExample()
{
   // delete _map;
	_animationSelector->close();
    shutdown();
}

void AnimationExample::keyPressEvent ( QKeyEvent * keyEvent )
{
	qDebug("[AnimationExample::keyPressEvent]");
	switch( keyEvent->key() )
    {
	case Qt::Key_C:
		_character->startAnimation("Greeting");
		_character->queueAnimation("Asking");
		_character->queueAnimation("GoodBye");
		_character->queueAnimation("Walk");
		break;
    case Qt::Key_A:
		_character->startAnimation("Asking");
        break;
	case Qt::Key_G:
		_character->startAnimation("GoodBye");
        break;
	case Qt::Key_O:
		_character->startAnimation("Greeting");
		break;
	case Qt::Key_S:
		_character->stopAnimation();
		break;
	case Qt::Key_W:
		_character->startAnimation("Walk", true);
        break;
    case Qt::Key_Escape:
        exit(0);
        break;
    }
}
