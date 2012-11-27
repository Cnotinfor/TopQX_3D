#include "PathFinderExample.h"
#include "PathFinderState.h"
// Cnoti3D
#include <OgreWidget>
// Ogre
#include <OgreResourceGroupManager.h>
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

// Create menus/buttons using only code (http://www.cegui.org.uk/wiki/index.php/The_Main_Menu)

PathFinderExample::PathFinderExample( QObject* parent )
: Cnoti3D::QtOgre( parent )
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
	loadScene("house.osm");
	loadScene("light_camera.osm");

	ogreWidget()->addCamera("invento");
	ogreWidget()->setActiveCamera("invento");

	_state = new PathFinderState(this);
	addState(_state);
	setInitialState( _state );
	startStates();

	ogreWidget()->show();
}

PathFinderExample::~PathFinderExample()
{
	shutdown();
}

