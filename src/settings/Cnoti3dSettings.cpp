#include "Cnoti3dSettings.h"
// Cnoti Settings
//#include <CnotiSettings>
// Ogre
#include <Ogre.h>
#include <OgreRoot.h>
#include <OgreRenderSystem.h>
// Qt
#include <QStringList>

namespace Settings
{

	/*!
		Update the options from the current (TODO: Nuno: maybe it's better without CnotiSettings)
	*/
//	void updateOgreRenderSettings(const CnotiSettings* settings)
//	{
//		Ogre::Root* ogre = Ogre::Root::getSingletonPtr();

//		const Ogre::RenderSystemList& renderList = ogre->getAvailableRenderers();
//		QString renderName = settings->option(Settings::RENDER_SYSTEM, Settings::DEFAULT_RENDER_SYSTEM).toString();

//		for(int i = 0; i < renderList.size(); i++)
//		{
//			Ogre::RenderSystem* render = renderList[i];
//			if( renderName.compare( QString::fromStdString( render->getName() ) ) == 0 )
//			{
//				QStringList options = settings->options( renderName );
//				QStringListIterator it(options);

//				while(it.hasNext())
//				{
//					QString option = it.next();
//					QString value = settings->option(option, "", renderName).toString();

//					render->setConfigOption( option.toStdString(), value.toStdString() );
//				}
//			}
//		}
//	}
}
