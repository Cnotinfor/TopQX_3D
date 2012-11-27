/**********************************************************************
 *<
	oFusion Scene Loader Pro (see License.txt)

	FILE: ogreAnimationCompiler.h

	DESCRIPTION: OE_XMLScene callbacks interface	

	IMPLEMENTED BY: Andres Carrera

	HISTORY: 

		Original implementation by Lasse Tassing (Channex)
		2005 ITE ApS, Lasse Tassing

 *>	Copyright (c) 2006, All Rights Reserved.
 **********************************************************************/


#ifndef _IOSMSCENECALLBACKS_H_
#define _IOSMSCENECALLBACKS_H_

#include <deque>
#include "Ogre.h"

typedef std::deque<Ogre::SceneNode*>	NodeList;

class oPassAnimation {
public:

	oPassAnimation() : index(0), alphaLength(0) {}

	unsigned int index;
	// Alpha animation
	typedef std::map<float, float>	AlphaKeyFramesList;
	AlphaKeyFramesList	alphaKeyFrames;

	float alphaLength;
};

class oMaterialAnimation {
public:

	oMaterialAnimation() : techniqueIndex(0) {}

	std::string materialName;
	std::string animationName;

	unsigned int techniqueIndex;
	
	typedef std::deque<oPassAnimation> oPassAnimatorList;
	oPassAnimatorList passAnims;
};

// Callback interface. Use this if you want to set special properties on the 
// objects during creation (and/or read custom attribute values).
class OSMSceneCallbacks
{
public:
	virtual ~OSMSceneCallbacks() {};

	// Called when a node has been created
	virtual void OnNodeCreate(Ogre::SceneNode *pNode, TiXmlElement* pNodeDesc) {};

	// Called when an entity has been created
	virtual void OnEntityCreate(Ogre::Entity *pEntity, TiXmlElement* pEntityDesc) {};

	// Called when a camera has been created
	virtual void OnCameraCreate(Ogre::Camera *pCamera, TiXmlElement* pCameraDesc) {};

	// Called when a light has been created
	virtual void OnLightCreate(Ogre::Light *pLight, TiXmlElement* pLightDesc) {};

	// Called when a helper has been created
	virtual void OnHelperCreated(Ogre::SceneNode* pHelper, TiXmlElement* pHelperDesc) {};

	// Called when a shape has been loaded	
	virtual void OnShapeLoaded(const Ogre::SimpleSpline& spline, 
							   const Ogre::Vector3& position, 
							   const Ogre::Quaternion& rotation, 
							   const Ogre::Vector3& scale) {
		// The client should copy and store the "spline" contents in order to use it
		// Dont store a pointer to it, the "spline" object is destroyed at function return
	};

	// Called when a render to texture has been created
	virtual void OnRenderTextureCreate(Ogre::RenderTexture* pRenderTex, TiXmlElement* pRenderTexDesc) {};

	// Called when a static geometry has been created
	virtual bool OnStaticGeometryCreated(Ogre::StaticGeometry* pStatic, const NodeList& nodeList) 
	{ 
		// Return true to keep the added nodes (will be hidden)
		// Return false to delete the added nodes from the scene manager

		return false;
	}

	// Called when a Material animator has been loaded
	virtual void OnMaterialAnimatorLoaded(const oMaterialAnimation& materialAnim) {}

};

#endif // _IOSMSCENECALLBACKS_H_
