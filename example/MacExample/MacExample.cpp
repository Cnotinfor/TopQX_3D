#include "MacExample.h"
// Cnoti3D
#include <OgreWidget>
// Ogre
#include "Ogre.h"
// Qt
#include <QDebug>

Example::Example( QObject* parent )
: Cnoti3D::Cnoti3dQtOgre( parent )
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

    loadScene("batutas.osm");

	ogreWidget()->show();

}

Example::~Example()
{
	shutdown();
}
