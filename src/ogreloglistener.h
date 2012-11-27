#ifndef OGRELOGLISTENER_H
#define OGRELOGLISTENER_H

#include <OgreLog.h>
#include <OgreString.h>

namespace Cnoti3D
{

	class OgreLogListener : public Ogre::LogListener
	{
	public:
		OgreLogListener();
		~OgreLogListener();
		virtual void messageLogged (const Ogre::String &message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName);
	};

};
#endif // OGRELOGLISTENER_H
