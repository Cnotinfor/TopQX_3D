#include "PathFinderState.h"
// Cnoti3D
#include <QtOgre>
#include <OgreWidget>
#include <Character>
#include <CharacterMovement>
#include <OSceneManager>
#include <OSceneLoader>
#include <Grid>
#include <PathFinder>
// Qt
#include <QDebug>

PathFinderState::PathFinderState(Cnoti3D::QtOgre* qtOgre, QState *parent):
	State("PathFinderExample", qtOgre, parent),
	_grid (NULL),
	_pathFinder (NULL)
{
	_batutas = new Cnoti3D::Character("LittleMozart", qtOgre->ogreWidget()->ogreSceneManager(), "Idle", this);
	QStringList nameLst;
	nameLst << "batutas_head" << "batutas_body";
	_batutas->addNodeList(nameLst);
	_batutas->setAnimated(true);
	_batutas->movement_Ptr()->setSpeedMultiplier(4);
	qtOgre->addObject(_batutas);

//	_grid = new Cnoti3D::Grid();
//	_grid->Load("../Resources/House/house.ivgd");
//	qDebug() << _grid->Print().c_str();

	_pathFinder = new Cnoti3D::PathFinder();
	_pathFinder->loadMap("../Resources/House/house.ivgd");
	_pathFinder->addImpassableChar('#');

	loadHelpers();
}

void PathFinderState::onEntry(QEvent *event)
{
	State::onEntry(event);
	// Connect mouse events
	if(_qtOgre != NULL)
	{
		connect(_qtOgre->ogreWidget(), SIGNAL(mouseReleasedEvent(QMouseEvent*)), this, SLOT(mouseReleasedEvent(QMouseEvent*)));
	}
}

void PathFinderState::loadHelpers()
{
	qDebug()<<"[Scene3D::loadHelpers]";

	OSMScene::HelperList helperList = _qtOgre->oSceneManager_Ptr()->OScene_Ptr("house.osm")->getHelperList();
	OSMScene::HelperList::iterator iter;
	Ogre::Vector3 v;
	Ogre::Quaternion q;
	for( iter = helperList.begin(); iter != helperList.end(); ++iter )
	{
		QString name = iter->first.c_str();

		if( name == "dummy_flute_1" ||
			name == "dummy_flute_2" ||
			name == "dummy_flute_3" ||
			name == "dummy_flute_4" ||
			name == "dummy_flute_5" ||
			name == "dummy_flute_6" ||
			name == "dummy_flute_7" ||
			name == "dummy_flute_8" ||
			name == "dummy_center_9" )
		{
			v = iter->second->getPosition();
			_positions << QVector3D(v.x,v.y,v.z);
		}

		// position of grid
		if(name.contains("grid"))
		{
			// limit of grid
			if(name.contains("1"))
			{
				v = iter->second->getPosition();
				if(_pathFinder)
				{
					_pathFinder->setMapPoint1(v.x, v.z);
				}
				if(_grid)
				{
					_grid->setPoint1(v.x, v.z);
				}
			}
			else
			{
				v = iter->second->getPosition();
				if(_pathFinder)
				{
					_pathFinder->setMapPoint2(v.x, v.z);
				}
				if(_grid)
				{
					_grid->setPoint2(v.x, v.z);
					_grid->OpenDoor('0');
					_grid->OpenDoor('1');
					_grid->OpenDoor('2');
					_grid->OpenDoor('3');
					_grid->OpenDoor('4');
					_grid->OpenDoor('5');
					_grid->OpenDoor('6');
					_grid->OpenDoor('7');
					_grid->OpenDoor('8');
					_grid->OpenDoor('a');
					_grid->OpenDoor('b');
					_grid->OpenDoor('c');
					_grid->OpenDoor('d');
					_grid->OpenDoor('e');
					_grid->OpenDoor('f');
					_grid->OpenDoor('g');
					_grid->OpenDoor('h');
					_grid->OpenDoor('i');
					_grid->OpenDoor('j');
					_grid->OpenDoor('k');
					_grid->OpenDoor('l');
					_grid->OpenDoor('m');
					_grid->OpenDoor('n');
					_grid->OpenDoor('o');
					_grid->OpenDoor('p');
					_grid->OpenDoor('q');
					qDebug() << "[PathFinderState::loadHelperInfo] - Grid for house created";
				}
			}
		}
	}
	qDebug() << "[PathFinderState::loadHelpers] - Done" << _positions;
}

void PathFinderState::keyPressEvent ( QKeyEvent * keyEvent )
{
	QVector3D destination;
	// Get point to walk to
	switch (keyEvent->key())
	{
	case Qt::Key_1:
		destination = _positions.at(1);
		break;
	case Qt::Key_2:
		destination = _positions.at(2);
		break;
	case Qt::Key_3:
		destination = _positions.at(3);
		break;
	case Qt::Key_4:
		destination = _positions.at(4);
		break;
	case Qt::Key_5:
		destination = _positions.at(5);
		break;
	case Qt::Key_6:
		destination = _positions.at(6);
		break;
	case Qt::Key_7:
		destination = _positions.at(7);
		break;
	case Qt::Key_8:
		destination = _positions.at(8);
		break;
	case Qt::Key_9:
		destination = _positions.at(0);
		break;
	}

	qDebug() << "[PathFinderState::keyPressEvent] Dest:" << destination;

	if(_pathFinder)
	{
		// Move Batutas
		QList<QVector3D> walkPoints = _pathFinder->getPath(_batutas->position(), destination);
		_batutas->moveTo(walkPoints);
	}

	if(_grid)
	{
		// Calculate pass points
		QList<QVector3D> pointsList;
		QVector3D origin = _batutas->position();
		_grid->SetPlayer(origin.x(), origin.z());

		// algortim of a* return 0 is the position select is free and exists a path to arrive
		if(_grid->SetPosReal(destination.x(), destination.z()) != 0)
		{
			qWarning() << "[World::getPointsToMoveTo] Can't move there";
			return;
		}
		// recover all node of path
		float x,z;
		for(int i=1; i<_grid->getPathSize(); i++)
		{
			x = _grid->getPathRealX(i);
			z = _grid->getPathRealY(i);
			pointsList << QVector3D(x, 0, z);
			qDebug() << "[World::getPointsToMoveTo] Point added:" << pointsList.last();
		}

		 // Move Batutas
		_batutas->moveTo(pointsList);
	}
}

void PathFinderState::mouseReleasedEvent(QMouseEvent *mouseEvent)
{
	qDebug() << "[PathFinderState::mouseReleasedEvent]";
}
