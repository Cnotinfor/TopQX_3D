/**********************************************************************
 *<
	oFusion Scene Loader Pro (see License.txt)

	FILE: OgreOSMScene.cpp

	DESCRIPTION: Contains the OE_XMLScene reader (oe_scene)	

	IMPLEMENTED BY: Andres Carrera

	HISTORY: 

		Original implementation by Lasse Tassing (Channex)
		2005 ITE ApS, Lasse Tassing

 *>	Copyright (c) 2006, All Rights Reserved.
 **********************************************************************/
#include "stdafx.h"

#include "OgreOSMScene.h"
#include "OgreTagPoint.h"
#include "OSMRenderTexListener.h"

using namespace Ogre;

enum {
	SCENE_SKYPLANE = 1,
	SCENE_SKYBOX,
	SCENE_SKYDOME,
};

#define ENABLE_LOGMANAGER LogManager::getSingleton().setLogDetail(LL_NORMAL);
#define DISABLE_LOGMANAGER LogManager::getSingleton().setLogDetail(static_cast<LoggingLevel>(0));

unsigned long OSMScene::mNextGeneratedNameExt = 1;


OSMScene::OSMScene(SceneManager* pSceneMgr, RenderWindow* win)
{
	mSceneMgr = pSceneMgr;

	if(win)	
		mWindow = win;		
	else
		mWindow = Ogre::Root::getSingleton().getAutoCreatedWindow();
	
	mCallbacks = 0;
}

OSMScene::~OSMScene(void)
{	
	
}

// Init overloads - use either of them
bool OSMScene::initialise(const char* pszFilename, OSMSceneCallbacks* pCallbacks)
{
	// Hook up callback interface
	mCallbacks = pCallbacks;

	LogManager::getSingleton().logMessage("********************************");	
	LogManager::getSingleton().logMessage("**     oScene Loader Lib      **");
	LogManager::getSingleton().logMessage("********************************");
	
	String msg("oSceneLoader: Loading '");
	msg += pszFilename;
	msg += "' file";
	LogManager::getSingleton().logMessage(msg);

	String path;
	StringUtil::splitFullFilename(pszFilename, mSceneName, mFileExtension, path);
	//StringUtil::splitBaseFilename(pszFilename, mSceneName, mFileExtension);

	String resourceGroupName = pszFilename;

	String sceneFilename = pszFilename;
	
	DataStreamPtr pStream;

	if(mFileExtension == "zip") 
	{
		ResourceGroupManager::getSingleton().addResourceLocation(pszFilename, "Zip", resourceGroupName, true);

		sceneFilename = mSceneName + ".osm";

		pStream = ResourceGroupManager::getSingleton().openResource(sceneFilename, resourceGroupName);
	}
	else 
	{
		pStream = ResourceGroupManager::getSingleton().openResource(sceneFilename);
	}

	// Create new XML document
	mXMLDoc = TiXmlDocumentPtr(new TiXmlDocument());
	
	//DataStreamPtr pStream = ResourceGroupManager::getSingleton().openResource(pszFilename);
	if(!pStream->size())
	{
		mXMLDoc.setNull();

		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
					"oSceneLoader: Empty scene file", 
					"OSMScene::initialise");
		
	}			
				
	size_t iSize = pStream->size();
	char *pBuf = new char[iSize+1];
	memset(pBuf, 0, iSize+1);
	pStream->read(pBuf, iSize);
	pStream.setNull();
	mXMLDoc->Parse(pBuf);
	delete[] pBuf;
	
	// check for errors
	if(mXMLDoc->Error())
	{
		mXMLDoc.setNull();

		String errDesc = "oSceneLoader: Failed to load scene file, ";
		msg += mXMLDoc->ErrorDesc();

		OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
					errDesc.c_str(), 
					"OSMScene::initialise");		
	}

	TiXmlElement* rootElem = mXMLDoc->RootElement();

	const char* pszType = rootElem->Attribute("type");
	
	TiXmlElement* locations = rootElem->FirstChildElement("locations");

	if(locations || pszType)
	{
		String sceneType;

		if(pszType)
			sceneType = pszType;

		if(sceneType != "zip") {
			const String& grpName = ResourceGroupManager::getSingleton().findGroupContainingResource(sceneFilename);
			//const String& grpName = ResourceGroupManager::getSingleton().findGroupContainingResource(pszFilename);

			if(!grpName.empty()) {
				FileInfoListPtr fileInfo = ResourceGroupManager::getSingleton().findResourceFileInfo(grpName, sceneFilename);
				//FileInfoListPtr fileInfo = ResourceGroupManager::getSingleton().findResourceFileInfo(grpName, pszFilename);

				String strPath = (*fileInfo)[0].archive->getName();

				strPath = StringUtil::standardisePath(strPath);		

				for (TiXmlElement* pLocationElem = locations->FirstChildElement();
					pLocationElem != 0; pLocationElem = pLocationElem->NextSiblingElement())
				{
					// Ogre could cast an exception, in which case we just try to 
					// continue reading the other location paths
					try 
					{	
						const char *pszName = pLocationElem->Attribute("path");

						if(String(pszName) == "/")
							continue;

						String strDir = strPath + pszName;

						ResourceGroupManager::getSingleton().addResourceLocation(strDir, "FileSystem", resourceGroupName);


					}
					catch(...) {

					}
				}
			}
		}

		try {
			ResourceGroupManager::getSingleton().initialiseResourceGroup(resourceGroupName);
		}
		catch(...) {

		}
	}

	const char* pszVersion = rootElem->Attribute("version");

	if(pszVersion)
		mSceneHeader.version = Ogre::StringConverter::parseReal(pszVersion);
	
	const char* pszScaleRef = rootElem->Attribute("unitsPerMasterScale");

	if(pszScaleRef)
		mSceneHeader.unitsPerMasterRef = Ogre::StringConverter::parseReal(pszScaleRef);

	const char* pszMasterScaleType = rootElem->Attribute("masterScaleRef");

	if(pszMasterScaleType)
		mSceneHeader.masterScaleRef = pszMasterScaleType;

	const char* pszUnitType = rootElem->Attribute("unitType");
	
	if(pszUnitType)
		mSceneHeader.unitType = pszUnitType;

	return true;
}

// Declare all resources used in the scene
void OSMScene::declareResources(void)
{
	if(!mXMLDoc.isNull())
	{

		TiXmlElement* rootElem = mXMLDoc->RootElement();

		// Get mesh filename from entities
		TiXmlElement *pMeshNode = rootElem->FirstChildElement("entities");
		if(pMeshNode)
		{
			// Iterate all meshes, creating them. 
			for (TiXmlElement* pMeshElem = pMeshNode->FirstChildElement();
				pMeshElem != 0; pMeshElem = pMeshElem->NextSiblingElement())
			{	
				// Declare mesh resource
				const char *pszFileName = pMeshElem->Attribute("filename");	
				ResourceGroupManager::getSingleton().declareResource(pszFileName, "Mesh");
			}
		}
	}
}

// Create scene, optionally attaching it to a parent node
bool OSMScene::createScene(Ogre::SceneNode* pParent)
{
	if(!mXMLDoc.isNull())
	{
		String msg("oSceneLoader: Creating scene on '");
		msg += pParent ? pParent->getName() : "Root";
		msg += "' node";
		LogManager::getSingleton().logMessage(msg);

		mSceneRoot = pParent;

		bool bHandled = false;

		TiXmlElement* rootElem = mXMLDoc->RootElement();

		pParent = initSceneManager(rootElem, bHandled);
		
		if(pParent==NULL)
			pParent=mSceneMgr->getRootSceneNode()->createChildSceneNode();

		assert(pParent);

		TiXmlElement* list;

		// Entities
		list = rootElem->FirstChildElement("entities");
		if(list)
			createEntities(list, pParent);

		// lights
		list = rootElem->FirstChildElement("lights");
		if(list)
			createLights(list, pParent);

		if(!bHandled) {
			// cameras
			list = rootElem->FirstChildElement("cameras");
			if(list)
				createCameras(list, pParent);
		}

		// helpers
		list = rootElem->FirstChildElement("helpers");
		if(list)
			createHelpers(list, pParent);

		// shapes
		list = rootElem->FirstChildElement("shapes");
		if(list)
			createShapes(list);

		// material animations
		list = rootElem->FirstChildElement("materialAnimations");
		if(list)
			loadMaterialAnimations(list);

		// External skeletal animation files
		list = rootElem->FirstChildElement("skeletal_animations");
		if(list)
		{
			SkeletonHandles skelHandles;
			loadAnimations(list, skelHandles);

			if(!skelHandles.empty())
			{

				SceneManager::MovableObjectIterator it = mSceneMgr->getMovableObjectIterator("Entity");

				while(it.hasMoreElements()) {
					Entity* entity = (Entity*)it.getNext();

					if( entity->hasSkeleton() && (skelHandles.find(entity->getMesh()->getSkeleton()->getHandle()) != skelHandles.end()) )
						entity->refreshAvailableAnimationState();
				}
			}
		}

		/*
		try {
			// Entities
			list = rootElem->FirstChildElement("entities");
			if(list)
				createEntities(list, pParent);
		} catch(...)
		{
			LogManager::getSingleton().logMessage("Error while loading entities");
			
			OGRE_EXCEPT(Exception::ERR_RT_ASSERTION_FAILED,
				"oSceneLoader: Error while loading entities",
				"OSMScene::createScene");
		}
		
		try {
			// lights
			list = rootElem->FirstChildElement("lights");
			if(list)
				createLights(list, pParent);
		} catch(...)
		{
			LogManager::getSingleton().logMessage("Error while loading lights");

			OGRE_EXCEPT(Exception::ERR_RT_ASSERTION_FAILED,
				"oSceneLoader: Error while loading lights",
				"OSMScene::createScene");
		}
		
		if(!bHandled) {
			try {
				// cameras
				list = rootElem->FirstChildElement("cameras");
				if(list)
					createCameras(list, pParent);
			} catch(...)
			{
				LogManager::getSingleton().logMessage("Error while loading cameras");

				OGRE_EXCEPT(Exception::ERR_RT_ASSERTION_FAILED,
				"oSceneLoader: Error while loading cameras",
				"OSMScene::createScene");
			}
		}
		
		try {
			// helpers
			list = rootElem->FirstChildElement("helpers");
			if(list)
				createHelpers(list, pParent);
		} catch(...)
		{
			LogManager::getSingleton().logMessage("Error while loading helpers");

			OGRE_EXCEPT(Exception::ERR_RT_ASSERTION_FAILED,
				"oSceneLoader: Error while loading helpers",
				"OSMScene::createScene");
		}
		
		try {
			// shapes
			list = rootElem->FirstChildElement("shapes");
			if(list)
				createShapes(list);
		} catch(...)
		{
			LogManager::getSingleton().logMessage("Error while loading shapes");

			OGRE_EXCEPT(Exception::ERR_RT_ASSERTION_FAILED,
				"oSceneLoader: Error while loading shapes",
				"OSMScene::createScene");
		}
		
		try 
		{
			// material animations
			list = rootElem->FirstChildElement("materialAnimations");
			if(list)
				loadMaterialAnimations(list);
		}catch(...)
		{
			LogManager::getSingleton().logMessage("Error while loading material animations");

			OGRE_EXCEPT(Exception::ERR_RT_ASSERTION_FAILED,
				"oSceneLoader: Error while loading material animations",
				"OSMScene::createScene");
		}
		
		try {
			// External skeletal animation files
			list = rootElem->FirstChildElement("skeletal_animations");
			if(list)
			{
				SkeletonHandles skelHandles;
				loadAnimations(list, skelHandles);

				if(!skelHandles.empty())
				{

					SceneManager::MovableObjectIterator it = mSceneMgr->getMovableObjectIterator("Entity");

					while(it.hasMoreElements()) {
						Entity* entity = (Entity*)it.getNext();

						if( entity->hasSkeleton() && (skelHandles.find(entity->getMesh()->getSkeleton()->getHandle()) != skelHandles.end()) )
							entity->refreshAvailableAnimationState();
					}
				}
			}
		}catch(...)
		{
			LogManager::getSingleton().logMessage("Error while loading external animations");

			OGRE_EXCEPT(Exception::ERR_RT_ASSERTION_FAILED,
				"oSceneLoader: Error while loading external animations",
				"OSMScene::createScene");
		}
		*/

		// Set scene properties
		setSceneProperties(rootElem);

		// Static Geometry
		list = rootElem->FirstChildElement("staticGeometry");
		if(list)
			createStaticGeometry(list);

		LogManager::getSingleton().logMessage("********************************");
		LogManager::getSingleton().logMessage("** oSceneLoader: Scene loaded **");
		LogManager::getSingleton().logMessage("********************************");

		return true;
	}

	return false;

	
}

// Get list of cameras in this scene
OSMScene::CameraList& OSMScene::getCameraList(void)
{
	return mCameras;
}

// Get list of lights in this scene
OSMScene::LightList& OSMScene::getLightList(void)
{
	return mLights;
}

// Get list of lights in this scene
OSMScene::EntityList& OSMScene::getEntityList(void)
{
	return mEntities;
}

Ogre::SceneNode* OSMScene::createNode(TiXmlElement* pElem, Ogre::SceneNode* pSceneRoot)
{
	SceneNode *pNode=0;

	// Try to find the parent node
	const char *pszName	= pElem->Attribute("name");
	if(pszName == NULL) return NULL;

	// Check if this node has a parent
	const char *pszParent = pElem->Attribute("parent");						
	if(pszParent == NULL)
	{
		// Check if the scene node has already been created by a child
		if(mSceneMgr->hasSceneNode(pszName))
		{
			pNode = mSceneMgr->getSceneNode(pszName);
		} else
		{
			pNode = pSceneRoot->createChildSceneNode(pszName);
		}

	} else
	{
		SceneNode *pParent=0;

		if(mSceneMgr->hasSceneNode(pszParent))
		{
			pParent = mSceneMgr->getSceneNode(pszParent);

		}else 
		{
			// We try to create the parent node as child of root node.
			// Later when the parent (hopefully) is created, we can adjust it,
			// if it is child of another node.
			pParent = pSceneRoot->createChildSceneNode(pszParent);
		}

		if(mSceneMgr->hasSceneNode(pszName))
		{
			// Check if the scene node has already been created by a child
			// In this case we would have to change the parent.
			pNode = mSceneMgr->getSceneNode(pszName);

			// Get old parent (probably scene root)
			SceneNode* pOldParent = pNode->getParentSceneNode();

			// Remove this node
			pOldParent->removeChild(pNode);

			// Insert us as child on the "real" parent
			pParent->addChild(pNode);

		} else
		{
			pNode = pParent->createChildSceneNode(pszName);
		}
	}

	// Position
	TiXmlElement* posElem = pElem->FirstChildElement("position");
	if(posElem)
	{
		Vector3 pos;			
		pos.x = StringConverter::parseReal(posElem->Attribute("x"));
		pos.y = StringConverter::parseReal(posElem->Attribute("y"));
		pos.z = StringConverter::parseReal(posElem->Attribute("z"));
		pNode->setPosition(pos);		
	}

	// Rotation
	TiXmlElement* rotElem = pElem->FirstChildElement("rotation");
	if(rotElem)
	{				
		pNode->setOrientation(
			StringConverter::parseReal(rotElem->Attribute("w")),
			StringConverter::parseReal(rotElem->Attribute("x")),
			StringConverter::parseReal(rotElem->Attribute("y")),
			StringConverter::parseReal(rotElem->Attribute("z")));

	}
	
	// Scale
	TiXmlElement* scaleElem = pElem->FirstChildElement("scale");
	if(scaleElem)
	{
		Vector3 scale;
		scale.x = StringConverter::parseReal(scaleElem->Attribute("x"));
		scale.y = StringConverter::parseReal(scaleElem->Attribute("y"));
		scale.z = StringConverter::parseReal(scaleElem->Attribute("z"));
		pNode->setScale(scale);
	}

	// Notify
	if(mCallbacks)
		mCallbacks->OnNodeCreate(pNode, pElem);

	// Animation
	TiXmlElement* animList = pElem->FirstChildElement("animations");
	if(animList)
	{
		// 
		for (TiXmlElement* animElem = animList->FirstChildElement();
			animElem != 0; animElem = animElem->NextSiblingElement())
		{		
			// Get name of animation
			const char *pszName = animElem->Attribute("name");

			Animation *pAnim = 0;

			if(mSceneMgr->hasAnimation(pszName))
			{
				pAnim = mSceneMgr->getAnimation(pszName);
			}

			// If this animation has not been created yet, we create it
			if(pAnim == 0)
			{			
				float fLength = StringConverter::parseReal(animElem->Attribute("length"));
				pAnim = mSceneMgr->createAnimation(pszName, fLength);
				pAnim->setInterpolationMode(Animation::IM_LINEAR);				
			}		

			// Create animation track for this node
			NodeAnimationTrack *pTrack = pAnim->createNodeTrack(pAnim->getNumNodeTracks()+1, pNode);

			// Iterate all keyframes for this node
			for (TiXmlElement* pKeyframeElem = animElem->FirstChildElement();
					pKeyframeElem != 0; pKeyframeElem = pKeyframeElem->NextSiblingElement())
			{
				float fTime=StringConverter::parseReal(pKeyframeElem->Attribute("time"));
				TransformKeyFrame *pKeyFrame = pTrack->createNodeKeyFrame(fTime);

				// Position
				TiXmlElement* posElem = pKeyframeElem->FirstChildElement("position");
				if(posElem)
				{
					Vector3 trans;			
					trans.x = StringConverter::parseReal(posElem->Attribute("x"));
					trans.y = StringConverter::parseReal(posElem->Attribute("y"));
					trans.z = StringConverter::parseReal(posElem->Attribute("z"));				
					pKeyFrame->setTranslate(trans);
				}

				// Rotation
				TiXmlElement* rotElem = pKeyframeElem->FirstChildElement("rotation");
				if(rotElem)
				{		
					Quaternion qRot;
					qRot.x = StringConverter::parseReal(rotElem->Attribute("x"));
					qRot.y = StringConverter::parseReal(rotElem->Attribute("y"));
					qRot.z = StringConverter::parseReal(rotElem->Attribute("z"));		
					qRot.w = StringConverter::parseReal(rotElem->Attribute("w"));		
					pKeyFrame->setRotation(qRot);
				}
				
				// Scale
				TiXmlElement* scaleElem = pKeyframeElem->FirstChildElement("scale");
				if(scaleElem)
				{
					Vector3 scale;
					scale.x = StringConverter::parseReal(scaleElem->Attribute("x"));
					scale.y = StringConverter::parseReal(scaleElem->Attribute("y"));
					scale.z = StringConverter::parseReal(scaleElem->Attribute("z"));				
					pKeyFrame->setScale(scale);
				}
			}		
		}
	}

	return pNode;
}

// Create SceneManager
Ogre::SceneNode* OSMScene::initSceneManager(TiXmlElement* sceneProp, bool& bHandled)
{
	assert(sceneProp);

	SceneType sceneType = ST_GENERIC;

	TiXmlElement* sceneMgrElem = sceneProp->FirstChildElement("sceneManager");

	if(mSceneMgr == NULL)
	{
		if(sceneMgrElem)
		{
			// Scene manager
			int type = Ogre::StringConverter::parseInt(sceneMgrElem->Attribute("type"));
			sceneType = static_cast<Ogre::SceneType>(1 << (type));
			mSceneMgr = Ogre::Root::getSingleton().createSceneManager(sceneType);

		} else
		{
			mSceneMgr = Root::getSingleton().createSceneManager(sceneType);
		}
	}
	
	assert(mSceneMgr);

	if(mSceneRoot == NULL) {
		StringUtil::StrStreamType nodeName;
		nodeName << "OSM_" << mSceneName << "_" << mNextGeneratedNameExt++;

		mSceneRoot = mSceneMgr->getRootSceneNode()->createChildSceneNode(nodeName.str());
	}

	//SceneNode* pSceneRoot = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	assert(mSceneRoot);	

	if(sceneMgrElem)
	{
		// Scene shadows
		TiXmlElement* shadowsElem = sceneProp->FirstChildElement("shadowTechnique");
		if(shadowsElem)
		{
			int type = StringConverter::parseInt(shadowsElem->Attribute("type"));

			ShadowTechnique shadowType = Ogre::SHADOWTYPE_NONE;

			if(type > 0 && type < 10) {
				// Serializer version 2 (Eihort shadow type)
				switch(type)
				{
				case 1:
					shadowType = Ogre::SHADOWTYPE_STENCIL_MODULATIVE;
					break;

				case 2:
					shadowType = Ogre::SHADOWTYPE_STENCIL_ADDITIVE;
					break;

				case 3:
					shadowType = Ogre::SHADOWTYPE_TEXTURE_MODULATIVE;
					break;

				case 4:
					shadowType = Ogre::SHADOWTYPE_TEXTURE_ADDITIVE;
					break;
				}		

			}
			else {
				// Serializer version 1 (Dagon based shadow type)
				shadowType = static_cast<Ogre::ShadowTechnique>(type);
			}

			mSceneMgr->setShadowTechnique(shadowType);

			int tex_size = StringConverter::parseInt(shadowsElem->Attribute("tex_size"));
			int tex_count = StringConverter::parseInt(shadowsElem->Attribute("tex_count"));

			mSceneMgr->setShadowTextureSettings(tex_size, tex_count);

			// Shadow Color
			TiXmlElement* colorElem = shadowsElem->FirstChildElement("color");
			if(colorElem)
			{	
				ColourValue color;
				color.r = StringConverter::parseReal(colorElem->Attribute("r"));
				color.g = StringConverter::parseReal(colorElem->Attribute("g"));
				color.b = StringConverter::parseReal(colorElem->Attribute("b"));

				mSceneMgr->setShadowColour(color);
			

			}

			// Scene fog
			TiXmlElement* fogElem = sceneProp->FirstChildElement("fogMode");
			if(fogElem)
			{
				int type = StringConverter::parseInt(fogElem->Attribute("type"));
				FogMode mode = static_cast<FogMode>(type);

				float density = StringConverter::parseReal(fogElem->Attribute("density"));
				float linearStart = StringConverter::parseReal(fogElem->Attribute("linearStart"));
				float linearEnd = StringConverter::parseReal(fogElem->Attribute("linearEnd"));

				ColourValue color;

				// Fog Color
				TiXmlElement* colorElem = fogElem->FirstChildElement("color");
				if(colorElem)
				{
					color.r = StringConverter::parseReal(colorElem->Attribute("r"));
					color.g = StringConverter::parseReal(colorElem->Attribute("g"));
					color.b = StringConverter::parseReal(colorElem->Attribute("b"));			
				}

				mSceneMgr->setFog(mode, color, density, linearStart, linearEnd);
			}

			// World Geometry
			const char* worldGeometry = sceneMgrElem->Attribute("worldGeometry");
			if(worldGeometry != NULL)
			{
				// Some scene managers need cameras created before the world geometry
				TiXmlElement* list = sceneProp->FirstChildElement("cameras");
				if(list)
					createCameras(list, mSceneRoot);

				if(sceneType == ST_EXTERIOR_CLOSE)
				{

					if(mCameras.empty()) 
					{

						OGRE_EXCEPT(Exception::ERR_RT_ASSERTION_FAILED,
							"oSceneLoader: Scene dont contain cameras, \
							Terrain Scene Manager needs a camera to initialize",
							"OSMScene::createSceneManager");
					}

					Camera* primaryCamera = mCameras[0];

					mWindow->getViewport(0)->setCamera(primaryCamera);

					mSceneMgr->setOption("PrimaryCamera", primaryCamera);
				}

				// cameras created, the main scene loader method should not re-parse the list
				bHandled = true;

				mSceneMgr->setWorldGeometry(worldGeometry);
			}
		}
	}


	return mSceneRoot;
}

// Set Scene Properties
void OSMScene::setSceneProperties(TiXmlElement* sceneProp)
{
	// Ambient light Color
	TiXmlElement* colorElem = sceneProp->FirstChildElement("lightColor");
	if(colorElem)
	{	
		ColourValue color;
		color.r = StringConverter::parseReal(colorElem->Attribute("r"));
		color.g = StringConverter::parseReal(colorElem->Attribute("g"));
		color.b = StringConverter::parseReal(colorElem->Attribute("b"));

		mSceneMgr->setAmbientLight(color);
	}

	// Background Color
	colorElem = sceneProp->FirstChildElement("bkgcolor");
	if(colorElem && mWindow)
	{	
		int numViewports = mWindow->getNumViewports();
		if(numViewports)
		{
			Ogre::ColourValue color;
			color.r = StringConverter::parseReal(colorElem->Attribute("r"));
			color.g = StringConverter::parseReal(colorElem->Attribute("g"));
			color.b = StringConverter::parseReal(colorElem->Attribute("b"));			
		
			for(int i=0; i<numViewports; ++i)
				mWindow->getViewport(i)->setBackgroundColour(color);
		}
	}

	// Scene sky
	TiXmlElement* skyElem = sceneProp->FirstChildElement("skyTechnique");
	if(skyElem)
	{
		int type = StringConverter::parseInt(skyElem->Attribute("type"));
		String materialName = skyElem->Attribute("material");
		
		if(!materialName.empty() && materialName != " ") {
			try {
				bool drawFirst = StringConverter::parseBool(skyElem->Attribute("drawFirst"));
				float tiling = StringConverter::parseReal(skyElem->Attribute("tiling"));
				float scale = StringConverter::parseReal(skyElem->Attribute("scale"));
				float dist = StringConverter::parseReal(skyElem->Attribute("dist"));
				float bow = StringConverter::parseReal(skyElem->Attribute("bow"));
				int xSegments = StringConverter::parseInt(skyElem->Attribute("xSegments"));
				int ySegments = StringConverter::parseInt(skyElem->Attribute("ySegments"));
				Ogre::Quaternion quat(Quaternion::IDENTITY);
				Ogre::Plane plane;
				plane.d = dist;
				plane.normal = -(Vector3::UNIT_Y);

				switch(type) {

				case SCENE_SKYPLANE:

					mSceneMgr->setSkyPlane(true, plane, materialName, scale, 
										   tiling, drawFirst, bow, xSegments, ySegments);

					mSceneMgr->setSkyBox(false, "");
					mSceneMgr->setSkyDome(false, "");

					break;

				case SCENE_SKYBOX:

					mSceneMgr->setSkyBox(true, materialName, dist, drawFirst, quat);
					mSceneMgr->setSkyPlane(false, plane, "");
					mSceneMgr->setSkyDome(false, "");

					break;

				case SCENE_SKYDOME:

					mSceneMgr->setSkyDome(true, materialName, bow, tiling, dist,
						drawFirst, quat, xSegments, ySegments);

					mSceneMgr->setSkyPlane(false, plane, "");
					mSceneMgr->setSkyBox(false, "");

					break;

				}
			}
			catch(Exception& /*e*/) {
				// Sky Material might not be in the resource list
				// More details were logged in the log file
			}
		}
	}
}

// Create all entities in scene
void OSMScene::createEntities(TiXmlElement* pEntityNode, Ogre::SceneNode* pSceneRoot)
{
	// Iterate all meshes, creating them. 
	for (TiXmlElement* pMeshElem = pEntityNode->FirstChildElement();
			pMeshElem != 0; pMeshElem = pMeshElem->NextSiblingElement())
	{
		const char *pszName = pMeshElem->Attribute("name");
		const char *pszFileName = pMeshElem->Attribute("filename");

		// try to create the mesh
		Entity *pEntity = mSceneMgr->createEntity(pszName, pszFileName);			
		if(pEntity==0) continue;

		// Check if the object should cast shadows
		const char *pszCastShadows=pMeshElem->Attribute("CastShadows");
		if(pszCastShadows && stricmp(pszCastShadows, "no")==0) {
			pEntity->setCastShadows(false);	
		}
		else {
			// Enable auto edge list generation if needed by the mesh
			pEntity->getMesh()->setAutoBuildEdgeLists(true);
			pEntity->setCastShadows(true);	
		}

		const char* pszTagPoint = pMeshElem->Attribute("tagPoint");

		if(pszTagPoint)
		{
			const char* pszParentName = pMeshElem->Attribute("parent");

			if(mSceneMgr->hasEntity(pszParentName))
			{
				Entity* entity = mSceneMgr->getEntity(pszParentName);

				Vector3 pos, scale;
				Quaternion rot;

				// Position
				TiXmlElement* posElem = pMeshElem->FirstChildElement("position");
				if(posElem)
				{
					pos.x = StringConverter::parseReal(posElem->Attribute("x"));
					pos.y = StringConverter::parseReal(posElem->Attribute("y"));
					pos.z = StringConverter::parseReal(posElem->Attribute("z"));

				} else
				{
					pos = Vector3::ZERO;
				}

				// Rotation
				TiXmlElement* rotElem = pMeshElem->FirstChildElement("rotation");
				if(rotElem)
				{
					rot.w = StringConverter::parseReal(rotElem->Attribute("w"));
					rot.x = StringConverter::parseReal(rotElem->Attribute("x"));
					rot.y = StringConverter::parseReal(rotElem->Attribute("y"));
					rot.z = StringConverter::parseReal(rotElem->Attribute("z"));

				} else 
				{
					rot = Quaternion::IDENTITY;
				}

				// Scale
				TiXmlElement* scaleElem = pMeshElem->FirstChildElement("scale");
				if(scaleElem)
				{
					scale.x = StringConverter::parseReal(scaleElem->Attribute("x"));
					scale.y = StringConverter::parseReal(scaleElem->Attribute("y"));
					scale.z = StringConverter::parseReal(scaleElem->Attribute("z"));

				} else
				{
					scale = Vector3::UNIT_SCALE;
				}

				TagPoint* tp = entity->attachObjectToBone(pszTagPoint, pEntity, rot, pos);

				tp->setScale(scale);
				tp->setBindingPose();
			}

		} else 
		{
			// Create node with full information
			SceneNode *pObjNode = createNode(pMeshElem, pSceneRoot);

			// Attach the mesh entity to node
			pObjNode->attachObject(pEntity);
		}

		// Notify
		if(mCallbacks)
			mCallbacks->OnEntityCreate(pEntity, pMeshElem);

		// Add to entity list
		mEntities.push_back(pEntity);
	}
}

// Create all Lights in scene
void OSMScene::createLights(TiXmlElement* pLightNode, Ogre::SceneNode* pSceneRoot)
{
	// Iterate all Lights, creating them. We do not attach them yet, since
	// we need to make sure all potential parent entities have been created.
	for (TiXmlElement* pLightElem = pLightNode->FirstChildElement();
			pLightElem != 0; pLightElem = pLightElem->NextSiblingElement())
	{
		const char *pszName = pLightElem->Attribute("name");

		Light *pLight = mSceneMgr->createLight(pszName);
		if(pLight==0) continue;

		// Figure out which type of light we are using
		const char *pszType = pLightElem->Attribute("type");
		if(stricmp(pszType, "omni")==0)
		{
			pLight->setType(Light::LT_POINT);
		} else if(stricmp(pszType, "spot")==0)
		{	
			pLight->setType(Light::LT_SPOTLIGHT);
			pLight->setSpotlightRange(
				Degree(StringConverter::parseReal(pLightElem->Attribute("hotspot"))),
				Degree(StringConverter::parseReal(pLightElem->Attribute("falloff"))));
			pLight->setDirection(0,0,-1);

		}  else if(stricmp(pszType, "directional")==0)
		{
			pLight->setType(Light::LT_DIRECTIONAL);
		}

		// Check if the light should be on
		const char *pszOn = pLightElem->Attribute("on");
		if(pszOn!=0 && stricmp(pszOn, "true")==0)
			pLight->setVisible(true);
		else
			pLight->setVisible(false);

		// Check if the object should cast shadows
		const char *pszCastShadows = pLightElem->Attribute("CastShadows");
		if(pszCastShadows && stricmp(pszCastShadows, "no")==0)
			pLight->setCastShadows(false);	
		else
			pLight->setCastShadows(true);	

		float lightIntens = 1.0f;

		const char* pszIntensity = pLightElem->Attribute("intensity");

		if(pszIntensity) {
			lightIntens = StringConverter::parseReal(pszIntensity);

			if(lightIntens > 1.0f)
				lightIntens = 1.0f;

			if(lightIntens < 0.0f)
				lightIntens = 0.0f;
		}

		// Diffuse Color
		TiXmlElement* colorElem = pLightElem->FirstChildElement("color");
		if(colorElem)
		{					
			pLight->setDiffuseColour(
				StringConverter::parseReal(colorElem->Attribute("r")) * lightIntens,
				StringConverter::parseReal(colorElem->Attribute("g")) * lightIntens,
				StringConverter::parseReal(colorElem->Attribute("b")) * lightIntens);
		}

		// Specular Color
		TiXmlElement* specularElem = pLightElem->FirstChildElement("specular");
		if(specularElem)
		{						
			pLight->setSpecularColour(
				StringConverter::parseReal(specularElem->Attribute("r")) * lightIntens,
				StringConverter::parseReal(specularElem->Attribute("g")) * lightIntens,
				StringConverter::parseReal(specularElem->Attribute("b")) * lightIntens);
		}

		// Attenuation
		TiXmlElement* attenElem = pLightElem->FirstChildElement("attenuation");
		if(attenElem) {
			pLight->setAttenuation(
				StringConverter::parseReal(attenElem->Attribute("range")),
				StringConverter::parseReal(attenElem->Attribute("constant")),
				StringConverter::parseReal(attenElem->Attribute("linear")),
				StringConverter::parseReal(attenElem->Attribute("quadratic")));
		}

		// Create node with full information
		SceneNode* pLightNode = createNode(pLightElem, pSceneRoot);

		// Attach the Light entity to node
		pLightNode->attachObject(pLight);

		// Target
		TiXmlElement* targetElem = pLightElem->FirstChildElement("target");
		if(targetElem)
		{	
			// Create node with full information
			SceneNode* pTargetNode = createNode(targetElem, pSceneRoot);
			pLightNode->setAutoTracking(true, pTargetNode);
		}		

		// Notify
		if(mCallbacks)
			mCallbacks->OnLightCreate(pLight, pLightElem);

		// Add to light list
		mLights.push_back(pLight);
	}	
}

// Create all Cameras in scene
void OSMScene::createCameras(TiXmlElement* pCameraNode, Ogre::SceneNode* pSceneRoot)
{
	// Iterate all Cameras, creating them. We do not attach them yet, since
	// we need to make sure all potential parent entities have been created.
	for (TiXmlElement* pCameraElem = pCameraNode->FirstChildElement();
			pCameraElem != 0; pCameraElem = pCameraElem->NextSiblingElement())
	{
		const char *pszName = pCameraElem->Attribute("name");

		// Create camera
		Camera* pCamera = mSceneMgr->createCamera(pszName);
		if(pCamera == 0) continue;

		// Set Field of View on camera
		pCamera->setFOVy(Radian(StringConverter::parseReal(pCameraElem->Attribute("FOV"))));

		TiXmlElement* pClipElem = pCameraElem->FirstChildElement("clipPlane");

		if(pClipElem) {
			const char* pszNearClip = pClipElem->Attribute("NEAR");

			if(pszNearClip)
				pCamera->setNearClipDistance(StringConverter::parseReal(pszNearClip));
			else
				pCamera->setNearClipDistance(5.0f);


			const char* pszFarClip = pClipElem->Attribute("FAR");

			if(pszFarClip)
				pCamera->setFarClipDistance(StringConverter::parseReal(pszFarClip));
		}

		// Create node with full information
		SceneNode* pCameraNode = createNode(pCameraElem, pSceneRoot);

		// Attach the Camera entity to node
		pCameraNode->attachObject(pCamera);

		// Target
		TiXmlElement* targetElem = pCameraElem->FirstChildElement("target");
		if(targetElem)
		{	
			// Create node with full information
			SceneNode* pTargetNode = createNode(targetElem, pSceneRoot);
			pCameraNode->setAutoTracking(true, pTargetNode);
		}

		if(!mWindow)
		{

			OGRE_EXCEPT(Exception::ERR_RT_ASSERTION_FAILED,
				"oSceneLoader: RenderWindow not valid, \
				If the RenderWindow was not auto created, \
				you must add it in the OSMScene object contructor",
				"OSMScene::createCameras");

		}

		// If viewport(s) are not present, create a default viewport
		if(mWindow->getNumViewports() == 0)
		{
			Viewport* vp = mWindow->addViewport(pCamera);

			// Alter the camera aspect ratio to match the viewport
			pCamera->setAspectRatio(
				Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
		}


		// Notify
		if(mCallbacks)
			mCallbacks->OnCameraCreate(pCamera, pCameraElem);

		// Add to camera list
		mCameras.push_back(pCamera);
	}
	
}

void OSMScene::createHelpers(TiXmlElement* pHelperNode, Ogre::SceneNode* pSceneRoot)
{
	// Iterate all helpers, creating them. 
	for (TiXmlElement* pHelperElem = pHelperNode->FirstChildElement();
			pHelperElem != 0; pHelperElem = pHelperElem->NextSiblingElement())
	{
		// Helper objects are scene nodes, create node with full information
		SceneNode* pHelperNode = createNode(pHelperElem, pSceneRoot);

		// Notify
		if(mCallbacks)
			mCallbacks->OnHelperCreated(pHelperNode, pHelperElem);
		
		mHelper.insert( std::make_pair( pHelperElem->Attribute("name"), pHelperNode ) ); //Cnoti
	}
}

void OSMScene::createShapes(TiXmlElement* pShapeNode)
{
	// Spline and Shapes are loaded as SimpleSpline (Catmull-Rom splines)
	// SimpleSpline objects are not managed by the resource system or the scene manager	
	// Because of this, loaded spline shapes are constructed as SplineObjects in the stack
	// and sent to the client in the OnShapeLoaded method
	// The client should copy and store the SimpleSpline object in order to use it

	// Load spline shapes only if the client uses notifications
	if(mCallbacks) {

		// Iterate all shapes, creating them. 
		for (TiXmlElement* pShapeElem = pShapeNode->FirstChildElement();
				pShapeElem != 0; pShapeElem = pShapeElem->NextSiblingElement())
		{
			const char* pszName = pShapeElem->Attribute("name");

			Vector3 pos, scale;
			Quaternion rot;

			// Position
			TiXmlElement* posElem = pShapeElem->FirstChildElement("position");
			if(posElem)
			{
				pos.x = StringConverter::parseReal(posElem->Attribute("x"));
				pos.y = StringConverter::parseReal(posElem->Attribute("y"));
				pos.z = StringConverter::parseReal(posElem->Attribute("z"));
			}

			// Rotation
			TiXmlElement* rotElem = pShapeElem->FirstChildElement("rotation");
			if(rotElem)
			{				
				rot.w = StringConverter::parseReal(rotElem->Attribute("w"));
				rot.x = StringConverter::parseReal(rotElem->Attribute("x"));
				rot.y = StringConverter::parseReal(rotElem->Attribute("y"));
				rot.z = StringConverter::parseReal(rotElem->Attribute("z"));
			}

			// Scale
			TiXmlElement* scaleElem = pShapeElem->FirstChildElement("scale");
			if(scaleElem)
			{
				scale.x = StringConverter::parseReal(scaleElem->Attribute("x"));
				scale.y = StringConverter::parseReal(scaleElem->Attribute("y"));
				scale.z = StringConverter::parseReal(scaleElem->Attribute("z"));
			}

			// Iterate all knot points for this shape
			TiXmlElement* pointList = pShapeElem->FirstChildElement("points");
			if(pointList)
			{
				SimpleSpline spline;

				// Load all knot points and then calculate the spline tangents
				spline.setAutoCalculate(false);

				for (TiXmlElement* pKnotElem = pointList->FirstChildElement();
						pKnotElem != 0; pKnotElem = pKnotElem->NextSiblingElement())
				{
					Vector3 knotPos;

					knotPos.x = StringConverter::parseReal(pKnotElem->Attribute("x"));
					knotPos.y = StringConverter::parseReal(pKnotElem->Attribute("y"));
					knotPos.z = StringConverter::parseReal(pKnotElem->Attribute("z"));

					spline.addPoint(knotPos);
				}

				// Calculate spline tangents
				spline.recalcTangents();

				// Notify
				mCallbacks->OnShapeLoaded(spline, pos, rot, scale);
			}
		}
	}

}

void OSMScene::createRenderTexture(TiXmlElement* pRenderTexNode)
{
	// Iterate all render textures, creating them. 
	for (TiXmlElement* pRttElem = pRenderTexNode->FirstChildElement();
			pRttElem != 0; pRttElem = pRttElem->NextSiblingElement())
	{
		oRenderTextureProps rtProps;

		const char* pszName = pRttElem->Attribute("name");

		if(pszName) 
		{
			rtProps.name = pszName;

			const char* pszCamName = pRttElem->Attribute("cameraName");

			if(pszCamName)
				rtProps.camName = pszCamName;

			const char* pszSchemeName = pRttElem->Attribute("schemeName");

			if(pszSchemeName)
				rtProps.schemeName = pszSchemeName;

			rtProps.width = Ogre::StringConverter::parseInt(pRttElem->Attribute("width"));
			rtProps.height = Ogre::StringConverter::parseInt(pRttElem->Attribute("height"));

			rtProps.pixelFmt = Ogre::StringConverter::parseInt(pRttElem->Attribute("pixelFormat"));
			rtProps.type = Ogre::StringConverter::parseInt(pRttElem->Attribute("type"));

			rtProps.clearBkg = Ogre::StringConverter::parseBool(pRttElem->Attribute("clearBackground"));

			TiXmlElement* colorElem = pRttElem->FirstChildElement("backgroundColor");

			if(colorElem)
			{
				rtProps.bkgColor.r = StringConverter::parseReal(pRttElem->Attribute("r"));
				rtProps.bkgColor.g = StringConverter::parseReal(pRttElem->Attribute("g"));
				rtProps.bkgColor.b = StringConverter::parseReal(pRttElem->Attribute("b"));
			}

			TiXmlElement* nodesElem = pRttElem->FirstChildElement("showNodes");

			if(nodesElem)
			{
				// Iterate all static geometry groups, creating them.
				for(TiXmlElement* pNodeElem = nodesElem->FirstChildElement();
					pNodeElem != 0; pNodeElem = pNodeElem->NextSiblingElement())
				{
					const char* pszName = pNodeElem->Attribute("name");

					if(mSceneMgr->hasEntity(pszName))
					{
						Entity* entity = mSceneMgr->getEntity(pszName);

						rtProps.showEntityList.push_back(entity);
					}
				}
			}

			nodesElem = pRttElem->FirstChildElement("hideNodes");

			if(nodesElem)
			{
				// Iterate all static geometry groups, creating them.
				for(TiXmlElement* pNodeElem = nodesElem->FirstChildElement();
					pNodeElem != 0; pNodeElem = pNodeElem->NextSiblingElement())
				{
					const char* pszName = pNodeElem->Attribute("name");

					if(mSceneMgr->hasEntity(pszName))
					{
						Entity* entity = mSceneMgr->getEntity(pszName);

						rtProps.hideEntityList.push_back(entity);
					}
				}
			}

			if(!mWindow)
				return;

			RenderSystem* rend = Root::getSingleton().getRenderSystem();

			TexturePtr tex = Root::getSingleton().getTextureManager()->getByName(rtProps.name);
			assert(tex.isNull());

			Ogre::Camera* camera = 0;

			if(mSceneMgr->hasCamera(rtProps.camName))
			{
				camera = mSceneMgr->getCamera(rtProps.camName);

				TextureType texType = static_cast<TextureType>(rtProps.type);
				PixelFormat pixelFormat = static_cast<PixelFormat>(rtProps.pixelFmt);

				// Create the resource group, if its still not registered in the resource manager
				try {
					ResourceGroupManager::getSingleton().createResourceGroup(OSM_RENDERTEXTURE_GROUP);
				}
				catch(...) {

				}

				TexturePtr tex = Root::getSingleton().getTextureManager()->createManual(rtProps.name,
					OSM_RENDERTEXTURE_GROUP,
					texType,
					rtProps.width, 
					rtProps.height,
					0,
					pixelFormat,
					TU_RENDERTARGET);

				RenderTexture* rTex = tex->getBuffer()->getRenderTarget();

				if(rTex)
				{
					Viewport* vp = rTex->addViewport(camera);
					vp->setMaterialScheme(rtProps.schemeName);
					vp->setClearEveryFrame(rtProps.clearBkg);
					vp->setBackgroundColour(rtProps.bkgColor);
					vp->setOverlaysEnabled(false);

					setRenderTextureNodes(rTex, &rtProps.showEntityList, &rtProps.hideEntityList);

					// Notify
					if(mCallbacks)
						mCallbacks->OnRenderTextureCreate(rTex, pRttElem);
				}
			}
		}
	}
}

void OSMScene::createStaticGeometry(TiXmlElement* pStaticGeom) 
{
	// Iterate all static geometry groups, creating them.
	for (TiXmlElement* pSGElem = pStaticGeom->FirstChildElement();
			pSGElem != 0; pSGElem = pSGElem->NextSiblingElement())
	{

		const char* pszName = pSGElem->Attribute("name");

		StaticGeometry* sgGroup = mSceneMgr->createStaticGeometry(pszName);

		// Set the rendering distance
		const char* pszDist = pSGElem->Attribute("distance");
		sgGroup->setRenderingDistance(StringConverter::parseReal(pszDist));

		// Statig Geometry cast shadows setting
		const char *pszCastShadows = pSGElem->Attribute("castShadows");
		if(pszCastShadows && stricmp(pszCastShadows, "no")==0)
			sgGroup->setCastShadows(false);				
		else
			sgGroup->setCastShadows(true);

		// Region dimensions
		TiXmlElement* pSizeElem = pSGElem->FirstChildElement("regionSize");
		if(pSizeElem)
		{
			Vector3 size;
			size.x = StringConverter::parseReal(pSizeElem->Attribute("x"));
			size.y = StringConverter::parseReal(pSizeElem->Attribute("y"));
			size.z = StringConverter::parseReal(pSizeElem->Attribute("z"));

			sgGroup->setRegionDimensions(size);
		}

		NodeList sceneNodesList;

		// Iterate all scene nodes in this group and them to the static geometry object.
		TiXmlElement* pNodesElem = pSGElem->FirstChildElement("nodes");
		if(pNodesElem)
		{
			
			for (TiXmlElement* pNodeElem = pNodesElem->FirstChildElement();
					pNodeElem != 0; pNodeElem = pNodeElem->NextSiblingElement())
			{
				
				const char* pszNode = pNodeElem->Attribute("name");

				SceneNode* node = mSceneMgr->getSceneNode(pszNode);
				node->setVisible(false);

				sgGroup->addSceneNode(node);
				
				sceneNodesList.push_back(node);

			}
		}

		// Build the static geometry
		sgGroup->build();

		bool keepNodes = false;

		// Notify
		if(mCallbacks)
			keepNodes = mCallbacks->OnStaticGeometryCreated(sgGroup, sceneNodesList);

		if(!keepNodes)
		{
			NodeList::iterator it = sceneNodesList.begin();
			NodeList::iterator iend = sceneNodesList.end();

			for(; it!=iend; ++it)
			{
				SceneNode* node = (*it);
				mSceneMgr->destroySceneNode(node->getName());
			}
		}
	}

}

void OSMScene::loadAnimations(TiXmlElement* animationsNode, SkeletonHandles& handles) {

	// Iterate all skeletons, adding animations from ".anim" files
	for (TiXmlElement* pSkeletonElem = animationsNode->FirstChildElement();
			pSkeletonElem != 0; pSkeletonElem = pSkeletonElem->NextSiblingElement())
	{
		const char *pszName = pSkeletonElem->Attribute("name");

		if(pszName == 0)
			continue;

		const Ogre::SkeletonPtr& skel = Ogre::SkeletonManager::getSingleton().getByName(pszName);

		if(!skel.isNull())
		{

			for (TiXmlElement* animElem = pSkeletonElem->FirstChildElement();
				animElem != 0; animElem = animElem->NextSiblingElement())
			{
				const char* pszFileName = animElem->Attribute("name");

				if(pszFileName) 
				{
					OSMAnimSerializer animSerializer;

					animSerializer.addAnimation(skel, pszFileName);
				}
			}

			handles.insert(skel->getHandle());
		}
	}
}

void OSMScene::setRenderTextureNodes(RenderTexture* rTex, const EntityList* showNodes, const EntityList* hideNodes)
{
	int visibleNodes = showNodes->size();
	int hiddenNodes = hideNodes->size();		

	if(visibleNodes || hiddenNodes) 
	{
		oRenderTextureListener* rTexListener = 0;

		rTexListener = new oRenderTextureListener();

		assert(rTexListener);

		rTex->addListener(rTexListener);

		int i;

		for(i=0; i<hiddenNodes; ++i) {
			Entity* entity = (*hideNodes)[i];

			if(entity == NULL)
				continue;

			// Add entity to the listener's list
			rTexListener->addEntityToHideList(entity);

		}

		for(i=0; i<visibleNodes; ++i) {
			Entity* entity = (*showNodes)[i];

			if(entity == NULL)
				continue;

			// Add entity to the listener's list
			rTexListener->addEntityToShowList(entity);
		}
	}
}

void OSMScene::loadMaterialAnimations(TiXmlElement* animationsNode) {

	// Iterate all animations, adding them to the target material
	for (TiXmlElement* pAnimElem = animationsNode->FirstChildElement();
			pAnimElem != 0; pAnimElem = pAnimElem->NextSiblingElement())
	{
		const char* pszName = pAnimElem->Attribute("name");

		if(pszName == 0)
			continue;

		const MaterialPtr& material = MaterialManager::getSingleton().getByName(pszName);

		if(!material.isNull()) {
			// Animation
			TiXmlElement* animList = pAnimElem->FirstChildElement("animations");
			if(animList)
			{

				for (TiXmlElement* animElem = animList->FirstChildElement();
					animElem != 0; animElem = animElem->NextSiblingElement())
				{

					// Get name of animation
					const char* pszAnimName = animElem->Attribute("name");

					if(pszAnimName == 0)
						continue;

					for(TiXmlElement* techElem = animElem->FirstChildElement();
						techElem != 0; techElem = techElem->NextSiblingElement())
					{
						const char* pszIndex = techElem->Attribute("index");

#ifdef _WIN32
                        UINT idx = StringConverter::parseInt(pszIndex);
#else
                        unsigned int idx = StringConverter::parseInt(pszIndex);
#endif

						oMaterialAnimation materialAnim;
						materialAnim.materialName = pszName;
						materialAnim.animationName = pszAnimName;
						materialAnim.techniqueIndex = idx;

						Technique* tech = material->getTechnique(idx);

						for(TiXmlElement* passElem = techElem->FirstChildElement();
							passElem != 0; passElem = passElem->NextSiblingElement())
						{
#ifdef _WIN32
                           UINT index = StringConverter::parseInt(passElem->Attribute("index"));
#else
                           unsigned int index = StringConverter::parseInt(passElem->Attribute("index"));
#endif

							Pass* pass = tech->getPass(index);

							TiXmlElement* alphaAnim = passElem->FirstChildElement("alphaAnimation");
							if(alphaAnim) {
								oPassAnimation passAnim;
								passAnim.index = index;

								float length = StringConverter::parseReal(alphaAnim->Attribute("length"));

								passAnim.alphaLength = length;

								// Iterate all alpha keyframes
								for(TiXmlElement* pKeyFrameElem = alphaAnim->FirstChildElement();
									pKeyFrameElem != 0; pKeyFrameElem = pKeyFrameElem->NextSiblingElement())
								{
									float time = StringConverter::parseReal(pKeyFrameElem->Attribute("time"));
									float value = StringConverter::parseReal(pKeyFrameElem->Attribute("value"));

									passAnim.alphaKeyFrames[time] = value;
								}

								materialAnim.passAnims.push_back(passAnim);
							}
						}

						// Notify
						if(mCallbacks)
							mCallbacks->OnMaterialAnimatorLoaded(materialAnim);
					}
				}
			}
		}
	}

}

OSMScene::HelperList& OSMScene::getHelperList(void) //Cnoti
{
	return mHelper;
}

OSMAnimSerializer::OSMAnimSerializer() {
	mVersion = "[oFusion_Serializer_v1.0]";
}

void OSMAnimSerializer::addAnimation(Ogre::SkeletonPtr skel, const Ogre::String& filename) {

	SkeletonSerializer serializer;
	StringUtil::StrStreamType msg;
	msg << "Skeletal Animation: Loading " << filename;
	LogManager::getSingleton().logMessage(msg.str());

	DataStreamPtr stream = ResourceGroupManager::getSingleton().openResource(filename);

	// Check header
	readFileHeader(stream);

#ifdef _WIN32
        uint16 numBones;
#else
        UInt16 numBones;
#endif

	// Read number of bones needed for this animation
	readShorts(stream, &numBones, 1);

	if(numBones != skel->getNumBones())
	{
		OGRE_EXCEPT(Exception::ERR_RT_ASSERTION_FAILED,
					"oSceneLoader: Animation not valid for skeleton: " + skel->getName(), 
					"OSMScene::addAnimation");
	}

	size_t size = (stream->size() - stream->tell());

	unsigned char* buffer = new unsigned char[size];

	stream->read(buffer, size);

	DataStreamPtr bufferStream(new MemoryDataStream(buffer, size, true));

	serializer.importSkeleton(bufferStream, skel.get());
}
