#ifndef _OSMRENDERTEXLISTENER_H_
#define _OSMRENDERTEXLISTENER_H_

#include "OgreEntity.h"

typedef std::vector<Ogre::Entity*>	EntityList;

typedef struct TAG_RENDERTEXTURE_PROPS {
	Ogre::String name;
	Ogre::String camName;

	unsigned int width;
	unsigned int height;

	int type;
	int pixelFmt;

	bool clearBkg;

	Ogre::ColourValue bkgColor;

	EntityList showEntityList;
	EntityList hideEntityList;

	Ogre::String schemeName;
	
} oRenderTextureProps;

class oRenderTextureListener : public Ogre::RenderTargetListener {
public:

	typedef std::vector<Ogre::Entity*>	EntityList;

	void addEntityToShowList(Ogre::Entity* entity) { mShowEntityList.push_back(entity); }
	void addEntityToHideList(Ogre::Entity* entity) { mHideEntityList.push_back(entity); }

	EntityList& getShowEntityList(void) { return mShowEntityList; }
	EntityList& getHideEntityList(void) { return mHideEntityList; }

	void clearEntityLists(void)
	{
		clearShowEntityLists();
		clearHideEntityLists();
	}
	void clearShowEntityLists(void) { mShowEntityList.clear(); }
	void clearHideEntityLists(void) { mHideEntityList.clear(); }

	void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
	{
		// Hide non visible nodes
		EntityList::iterator it = mHideEntityList.begin();
		EntityList::iterator iend = mHideEntityList.end();

		for(; it != iend; ++it)
			(*it)->setVisible(false);

		// Show exclusive nodes
		it = mShowEntityList.begin();
		iend = mShowEntityList.end();

		for(; it != iend; ++it)
			(*it)->setVisible(true);
	}
	void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
	{
		// Show previously hidden nodes
		EntityList::iterator it = mHideEntityList.begin();
		EntityList::iterator iend = mHideEntityList.end();

		for(; it != iend; ++it)
			(*it)->setVisible(true);

		// Exclusive node list
		it = mShowEntityList.begin();
		iend = mShowEntityList.end();

		for(; it != iend; ++it)
			(*it)->setVisible(false);
	}

protected:
	
	//typedef std::deque<Ogre::Entity*> EntityList;
	EntityList mShowEntityList;
	EntityList mHideEntityList;
};

#endif // _OSMRENDERTEXLISTENER_H_
