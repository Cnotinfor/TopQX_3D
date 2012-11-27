/**********************************************************************
 *<
	oFusion Scene Loader Pro (see License.txt)

	FILE: OSMAnimSerializer.h

	DESCRIPTION: External Animation serializer include header

	IMPLEMENTED BY: Andres Carrera

 *>	Copyright (c) 2007, All Rights Reserved.
 **********************************************************************/

#ifndef _OSMANIMSERIALIZER_H_
#define _OSMANIMSERIALIZER_H_

class OSMAnimSerializer : public Ogre::Serializer {
public:

	OSMAnimSerializer();
	
	void addAnimation(Ogre::SkeletonPtr skel, const Ogre::String& filename);

};

#endif // _OSMANIMSERIALIZER_H_
