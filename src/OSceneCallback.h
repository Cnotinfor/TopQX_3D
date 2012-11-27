#ifndef __CNOTI_OSCENE_CALLBACKS_H_
#define __CNOTI_OSCENE_CALLBACKS_H_


#include "oSceneLoader/include/IOSMSceneCallbacks.h"

namespace Cnoti3D
{
	// Callback handler to post-process created objects
	class OSceneCallback : public OSMSceneCallbacks 
	{
	public:
		OSceneCallback();
		~OSceneCallback();

		/*
		// We override the OnCreate method for cameras (See IXMLSceneCallbacks class)
		void OnCameraCreate( Ogre::Camera* pCamera, TiXmlElement* pCameraDesc ) 
		{
			// If a camera of name "FirstCamera" is loaded, it will be set as the default current 
			if( pCamera->getName() == "CnotiCamera" )
				Ogre::Root::getSingleton().getAutoCreatedWindow()->getViewport(0)->setCamera( pCamera );
		}*/
	};
}

#endif	// __CNOTI_OSCENE_CALLBACKS_H_