#include "ogreloglistener.h"

// Qt
#include <QDebug>

// Ogre
#include "OgreLog.h"
#include "OgreString.h"

namespace Cnoti3D
{
	OgreLogListener::OgreLogListener()
	{
	}


	OgreLogListener::~OgreLogListener()
	{

	}

	void OgreLogListener::messageLogged (const Ogre::String &message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName)
	{
		switch((int)lml)
		{
		case Ogre::LML_TRIVIAL:
			qDebug() << "[OGRE]" << message.c_str();
			break;
		case Ogre::LML_NORMAL:
			qWarning() << "[OGRE]" << message.c_str();
			break;
		case Ogre::LML_CRITICAL:
			qCritical() << "[OGRE]" << message.c_str();
			break;
		}
	}
}
