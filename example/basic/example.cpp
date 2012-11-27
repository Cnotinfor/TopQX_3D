#include "Example.h"
#include <Object3DAbstract>
#include <QOgreGraphicsView>
#include <Object3D>
#include <Object3DMousable>
#include <OgreWidget>
#include <QOgreGraphicsScene>
// Ogre
#include "Ogre.h"
// Qt
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QListWidget>
#include <QMessageBox>
#include <QMouseEvent>
#include <QStringList>
#include <QWidget>
#include <QGraphicsGridLayout>

Example::Example( QObject* parent )
: Cnoti3D::Cnoti3dQtOgre( parent )
{
    qDebug()<<"Debug";
    qWarning()<<"Warning";
    qCritical()<<"Critical";

	initialise( 800, 600 );

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
