/**********************************************************************
 *<
	oFusion Scene Loader Pro (see License.txt)

	FILE: OgreOSMScene.h

	DESCRIPTION: OE_XMLScene reader (oe_scene) include header	

	IMPLEMENTED BY: Andres Carrera

	HISTORY: 

		Original implementation by Lasse Tassing (Channex)
		2005 ITE ApS, Lasse Tassing

 *>	Copyright (c) 2007, All Rights Reserved.
 **********************************************************************/


#ifndef _OGREOSMSCENE_H_
#define _OGREOSMSCENE_H_


#include "Ogre.h"
#include "OgreSharedPtr.h"
#include "../TinyXML/tinyxml.h"

#include "IOSMSceneCallbacks.h"
#include "OSMAnimSerializer.h"

#define OSM_RENDERTEXTURE_GROUP "OSM_RENDERTEXTURE_GROUP_"

class TiXmlDocumentPtr : public Ogre::SharedPtr<TiXmlDocument> {
public:
	TiXmlDocumentPtr() : Ogre::SharedPtr<TiXmlDocument>() {}	
	explicit TiXmlDocumentPtr(TiXmlDocument* rep) : Ogre::SharedPtr<TiXmlDocument>(rep) {}
	
};

typedef struct TAG_SCENE_HEADER_PROPS {
	TAG_SCENE_HEADER_PROPS() : version(0), unitsPerMasterRef(1) {}

	float version;

	float unitsPerMasterRef;

	Ogre::String	masterScaleRef;
	Ogre::String	unitType;

} OSceneHeader;

class OSMScene
{
public:

	typedef std::vector<Ogre::Camera*>	CameraList;
	typedef std::vector<Ogre::Light*>	LightList;
	typedef std::vector<Ogre::Entity*>	EntityList;
	typedef std::map<Ogre::String, Ogre::SceneNode*> HelperList;    //Cnoti

	OSMScene(Ogre::SceneManager* pSceneMgr = 0, Ogre::RenderWindow* win = 0);
	~OSMScene(void);

	// Initialise
	bool	initialise(const char* pszFilename, OSMSceneCallbacks* pCallbacks = NULL);

	// Declare all resources used in the scene
	void	declareResources(void);

	// Create scene, optionally attaching it to a parent node
	bool	createScene(Ogre::SceneNode* pParent = NULL);

	// Get list of cameras in this scene
	CameraList&	getCameraList(void);

	// Get list of lights in this scene
	LightList&	getLightList(void);

	// Get list of lights in this scene
	EntityList&	getEntityList(void);
	
	// Get list of Helpers in this scene
	HelperList&	getHelperList(void);	//Cnoti

	// Get created scene manager
	Ogre::SceneManager* getSceneManager(void) { return mSceneMgr; }

	const OSceneHeader& getSceneHeader(void) { return mSceneHeader; }

protected:
	// Create node from information
	Ogre::SceneNode* createNode(TiXmlElement* pElem, Ogre::SceneNode* pSceneRoot);

	// Create SceneManager
	Ogre::SceneNode* initSceneManager(TiXmlElement* sceneProp, bool& bHandled);
	//Ogre::SceneNode* createSceneManager(TiXmlElement* sceneProp, bool& bHandled);

	// Set scene properties
	void setSceneProperties(TiXmlElement* sceneProp);

	// Creation helpers		
	void	createEntities(TiXmlElement* pEntityNode, Ogre::SceneNode* pSceneRoot);
	void	createLights(TiXmlElement* pLightNode, Ogre::SceneNode* pSceneRoot);
	void	createCameras(TiXmlElement* pCameraNode, Ogre::SceneNode* pSceneRoot);
	void	createStaticGeometry(TiXmlElement* pStaticGeom);
	void	createHelpers(TiXmlElement* pLightNode, Ogre::SceneNode* pSceneRoot);
	void	createShapes(TiXmlElement* pHelperNode);
	void	createRenderTexture(TiXmlElement* pRenderTexNode);
	
	typedef std::set<unsigned int> SkeletonHandles;
	void	loadAnimations(TiXmlElement* animationsNode, SkeletonHandles& handles);

	void	loadMaterialAnimations(TiXmlElement* animationsNode);

	void	setRenderTextureNodes(Ogre::RenderTexture* rTex, const EntityList* showNodes, const EntityList* hideNodes);

	// Created objects
	CameraList	mCameras;
	LightList	mLights;
	EntityList	mEntities;
	HelperList	mHelper;    //Cnoti

	Ogre::SceneNode*	mSceneRoot;
	Ogre::String		mSceneName, mFileExtension;

	// Callback interface
	OSMSceneCallbacks* mCallbacks;

	// Scene manager
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;

	// Current Scene header properties
	OSceneHeader mSceneHeader;

	// Incremented count for next node name extension
	static unsigned long mNextGeneratedNameExt;

	// Scene XML document
	TiXmlDocumentPtr mXMLDoc;
};

#endif // _OGREOSMSCENE_H_
