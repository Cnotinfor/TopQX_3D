#include "QtOgre.h"
#include "Object3dAbstract.h"
#include "object3d.h"
#include "OSceneManager.h"
#include "ObjectManager.h"
#include "OgreWidget.h"
#include "Object3dMousable.h"
#include "Button2dManager.h"
#include "statemanager.h"
#include "state.h"
#include "OSceneLoader.h"
#include "interface/Fader.h"
#include "settings/Cnoti3dSettings.h"
#include "OgreLogListener.h"
// Qt
#include <QDebug>
#include <QTimer>
#include <QMutexLocker>
#include <QMouseEvent>
#include <QApplication>
#include <QToolTip>
#include <QHelpEvent>
#include <QList>
#include <QHash>
#include <QString>


class GraphicsSceneMouseEvent;


namespace Cnoti3D
{

/*!

*/
QtOgre::QtOgre( QObject* parent )
	: QObject( parent ),
	  Cnoti3d(),
	  _timer( NULL ),
	  _ogreWidget( NULL ),
	  _lastObjectEntered( NULL ),
	  _lastObject2dEntered( NULL ),
	  _buttonsMgr( NULL ),
	  _stateMgr( NULL ),
	  _fader( NULL ),
	  _fadeTime( 1.0 ),
	  _fullscreen( false )
{

}
/*!

*/
QtOgre::~QtOgre()
{

}

/*!
	Creates the view an scene to render Ogre3D and a timer to update the 3D view.

	The options is an Hashtable with keys and values from Ogre configuration file.
*/
void QtOgre::initialise( QHash<QString, QString> options )
{
	int width, height;

	qDebug() << "[Cnoti3dQtOgre::initialise]";

	// Log must be created before ogre Root is created
	Ogre::LogManager* logManager = new Ogre::LogManager();
	Ogre::Log* log = new Ogre::Log("", true, true);
	OgreLogListener* listener = new OgreLogListener();

	log->addListener(listener);
	logManager->setDefaultLog(log);

	if(options.contains( Settings::RENDER_VIDEO_MODE ) )
	{
		QString option = options.value( Settings::RENDER_VIDEO_MODE );
		QStringList dimensions = option.split( " x " );
		if(options.value(Settings::RENDER_SYSTEM).contains(Settings::DIRECT3D9))
		{
			dimensions.last() = dimensions.last().split(" @ ").first();
		}

		// Test if the split work correctly or if the setting is valid
		if(dimensions.size() == 2)
		{
			width = dimensions.first().toInt();
			height = dimensions.last().toInt();
		}
		else
		{
			width = Settings::DEFAULT_SCREEN_WIDTH;
			height = Settings::DEFAULT_SCREEN_HEIGHT;
		}
	}
	else
	{
		width = Settings::DEFAULT_SCREEN_WIDTH;
		height = Settings::DEFAULT_SCREEN_HEIGHT;
	}

	qDebug() << "[Cnoti3dQtOgre::initialise]" << width << "x" << height;
	qDebug() << "[Cnoti3dQtOgre::initialise]" << options;

	// Add VSync = Yes, to better performance
	options.insert( "vsync", "true" );

	//
	// Creates the GL context
	//
	_ogreWidget = new OgreWidget( options );
	_ogreWidget->initialise();
	if (options.value(Settings::RENDER_FULLSCREEN) == Settings::YES)
	{
#ifdef Q_WS_MAC
		_ogreWidget->setWindowState( Qt::WindowMaximized );
#else
		_ogreWidget->setWindowState( Qt::WindowFullScreen );
#endif
		_fullscreen = true;
	}
	else
	{
		_ogreWidget->resize(width, height);
		_fullscreen = false;
	}

	//
	// Loads resources from resource.cfg file
	//
	_ogreWidget->setupResources();

	//
	// Initialises the gui (needs to be after the resources)
	//
	_ogreWidget->initialiseGui(_ogreWidget->ogreRenderWindow());
	CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Size(_ogreWidget->width(), _ogreWidget->height()));

	//
	// Creates the timer to update the view
	//
	_timer = new QTimer(0);
	connect( _timer, SIGNAL( timeout() ), this, SLOT( update() ) );
	_timer->start(20);

	//
	// Create the oFusion scene loader
	//
	_oSceneLoaderMgr = new OSceneManager();

	//
	// Connect mouse event to check interection with 3D objects
	//
	connect(_ogreWidget, SIGNAL(mouseMovedEvent(QMouseEvent*)), this, SLOT(checkObjectMouseMoved(QMouseEvent*)));
	connect(_ogreWidget, SIGNAL(mouseReleasedEvent(QMouseEvent*)), this, SLOT(checkObjectMouseReleased(QMouseEvent*)));

	// Connect rezise and move events
	connect(_ogreWidget, SIGNAL(moved(QPoint)), this, SLOT(widgetMoved(QPoint)));
	connect(_ogreWidget, SIGNAL(resized(QSize)), this, SLOT(widgetResized(QSize)));

	// Starts the frame timer;
	_frameTimer.start();

	// initialise fader
	_fader = new Fader("Overlays/FadeInOut", "Materials/OverlayMaterial");
}

/*!
  To update internal objects (3d objects, etc)
*/
void QtOgre::update()
{
//qWarning() << "[Cnoti3dQtOgre::update]";
	qreal timeSinceLastFrame = ((qreal)_frameTimer.restart()) * 0.001f;
	_3dObjectsMgr->updateObjects(timeSinceLastFrame);
	_ogreWidget->updateOgre(timeSinceLastFrame);
	if(_fader)
	{
		_fader->fade(timeSinceLastFrame);
	}
}

/*!
	Shutdows the 3D
*/
void QtOgre::shutdown()
{
	if( _ogreWidget != NULL )
	{
		delete _timer;
		_timer = NULL;
	}
}

/*!
	Handles window resize.
*/
void QtOgre::resize( int width, int height )
{
	if( _ogreWidget != NULL )
	{
		_ogreWidget->resize( width, height );
	}
}

/*!
	Default implemenation does nothing
*/
void QtOgre::widgetMoved(QPoint p)
{

}

/*!
	Default implemenation does nothing
*/
void QtOgre::widgetResized(QSize s)
{

}

/*!
	Loads a 3D studioMax scene exported with oFusion
*/
void QtOgre::loadScene( const QString &filename)
{
	QMutexLocker locker(&_loadMutex);

	_oSceneLoaderMgr->loadSceneFromFile( filename, _ogreWidget->ogreSceneManager(), _ogreWidget->ogreRenderWindow() );

	// Gets light in the scene loaded
	OSMScene::LightList lightList = _oSceneLoaderMgr->OScene_Ptr(filename)->getLightList(); // Gets the vector of loaded ligths
	OSMScene::LightList::iterator iter;
	for( iter = lightList.begin(); iter != lightList.end(); ++iter )
	{
		_lightList <<  (*iter)->getName().c_str(); // Saves the name of the light
	}
}

/*!
Creates a new cnoti3dObject that can be inserted in the 3D world.
*/
Object3DAbstract* QtOgre::createObject(QString objectName, QString meshFile, QString animationFile, Clickable clickable)
{
	Object3D* object;
	if( !_3dObjectsMgr->isCnoti3dObject( objectName ) )
	{
		// create Ogre entity and attach it to a sceneNode
		Ogre::SceneManager* sceneMgr = _ogreWidget->ogreSceneManager();
		Ogre::Entity* entity = sceneMgr->createEntity( objectName.toStdString(), meshFile.toStdString() );
		Ogre::SceneNode* sceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode( objectName.toStdString() );
		sceneNode->attachObject( entity );
		// Creates the object
		if (clickable == notClickable)
		{
			object = new Object3D(entity);
		}else
		{
			object = new Object3DMousable(entity);
		}
		return object;//this->addObject(object);
	}
	return NULL;
}

Object3DAbstract* QtOgre::addObject(Object3DAbstract* object)
{
	if( !_3dObjectsMgr->isCnoti3dObject( object->name() ) )
	{
		_3dObjectsMgr->addObject3DAbstract( object );
		return object;
	}
	return NULL;
}

/*!
Creates an Cnoti3D Object and places it in the position x, y, z.
*/
Object3DAbstract* QtOgre::createObject(QString objectName, QString meshFile, QString animationFile, qreal x, qreal y, qreal z, Clickable clickable)
{
	Object3DAbstract* object = createObject( objectName, meshFile, animationFile, clickable);
	object->setPosition( QVector3D(x, y, z) );
	return object;
}

/*!
Creates an Cnoti3D Object and places it in the position x, y, z.
\param clickable is to distinguish from a clickable object our non-clickable object
*/
Object3DAbstract* QtOgre::createObject(QString objectName, QString meshFile, qreal x, qreal y, qreal z, Clickable clickable)
{
	Object3DAbstract* object = createObject( objectName, meshFile,"", clickable);
	object->setPosition( QVector3D(x, y, z) );
	return object;
}

/*!
  Creates a new Cnoti3d Object from an entity name.
  Returns pointer to new object or NULL if entity wasn't found.
*/
Object3DAbstract* QtOgre::createObject(QString entityName)
{
	if(_ogreWidget->ogreSceneManager()->hasEntity(entityName.toStdString()))
	{
		Ogre::Entity* entity = _ogreWidget->ogreSceneManager()->getEntity(entityName.toStdString());
		return new Object3D(entity);
	}
	return NULL;
}

/*!
Gets a Object by it's name
*/
Object3DAbstract* QtOgre::object(const QString& objectName)
{
	//Ogre::Entity* ent = _qOgreGL->ogreSceneManager()->getEntity( objectName.toStdString() );
	//return dynamic_cast<Object3DAbstract*>(ent);
	return _3dObjectsMgr->object3DAbstract(objectName);
}

/*!
Gets a Entity by it's name
*/
Ogre::Entity* QtOgre::entity(const QString& objectName)
{
	Ogre::Entity* tmpEntity = NULL;
	try
	{
		tmpEntity = _ogreWidget->ogreSceneManager()->getEntity( objectName.toStdString() );
	}catch(...)
	{
		qWarning()<<"[Cnoti3dQtOgre::entity]: entity ("+objectName+") not found";
		tmpEntity = NULL;
	}
	return tmpEntity;
}

/*!
Creates an Entity and attachs it to a scene node
*/
Ogre::Entity* QtOgre::createEntity(const QString& objectName, const QString& meshFile)
{
	QMutexLocker locker(&_loadMutex);
	Ogre::Entity* tmpEntity = NULL;
	if(!entity(objectName))
	{
		//            _qOgreGL->makeOgreCurrent(); //Info: Removed because of error in LoadEntity and consequences weren't properly tested
		tmpEntity = _ogreWidget->ogreSceneManager()->createEntity( objectName.toStdString(), meshFile.toStdString() );
		Ogre::SceneNode* sceneNode = _ogreWidget->ogreSceneManager()->getRootSceneNode()->createChildSceneNode( objectName.toStdString() );
		sceneNode->attachObject( tmpEntity );
	}
	return tmpEntity;
}

/*!
	Returns the first object behind the mouse pointer

	The mousePos is a position in scene coordinates.
*/
Object3DMousable* QtOgre::getObjectByMousePosition( const QPointF &mousePos )
{
	// Only can return an objcect 3D if the visible 2D buttons are not in modal state
	if(_buttonsMgr != NULL )
	{
		if(_buttonsMgr->isShowingModalButtons())
		{
			return NULL;
		}
	}
	// Test if there is an GraphicsItem at the mouse position
	// If it doesn't then test if there is a 3D object

	if(!has2dTargetAtPosition(QPoint(mousePos.x(), mousePos.y())))
	{
		return _ogreWidget->objectByMousePosition( mousePos, _3dObjectsMgr);
	}
	return NULL;
}


void QtOgre::loadResource(QString resourceFile)
{

}

void QtOgre::unloadResource(QString resourceFile)
{

}

/*!
  Adds a new state to the application (StateManager/StateMachine).
  If state Manager is not created it is created.
  Also it's set the pointer to the application.
*/
void QtOgre::addState(State *state)
{
	if(_stateMgr == NULL)
	{
		_stateMgr = new StateManager(this);
	}
	//state->setCnoti3dQtOgre_Ptr(this);
	_stateMgr->addState(state);
}

/*!
  Sets the initial state for the application (StateMachine).

  \sa addState
*/
void QtOgre::setInitialState(State *state)
{
	if(_stateMgr != NULL)
	{
		_stateMgr->setInitialState(state);
	}
	else
	{
		qWarning() << "[Cnoti3dQtOgre::setInitialState] Not possible to set inicial state, state manager is NULL";
	}
}

/*!
  Initiates the states (StateMachine) going to the initial state.

  \sa setInitialState
*/
void QtOgre::startStates()
{
	if(_stateMgr != NULL)
	{
		_stateMgr->start();
	}
	else
	{
		qWarning() << "[Cnoti3dQtOgre::setInitialState] Not possible to start state manager, is NULL";
	}
}

/*!
  Check if an object was clicked when the mouse released event occurred.
*/
void QtOgre::checkObjectMouseReleased(QMouseEvent* event)
{
	// Check if there isn't any 2D object underneath.
	if(!has2dTargetAtPosition(event->pos()))
	{
		// Check if there is an 3D object underneath before
		QPointF p = event->pos();
		Object3DMousable* object = getObjectByMousePosition( p );
		if(object != NULL)
		{
//			qDebug() << "[Cnoti3dQtOgre::checkObjectMouseReleased] " << object;
            object->onMouseReleaseEvent(event);
		}
//			qDebug() << "[Cnoti3dQtOgre::checkObjectMouseReleased] Has child - FALSE";
	}
//	else
//		qDebug() << "[Cnoti3dQtOgre::checkObjectMouseReleased] Has child - TRUE";
}

/*!
  Check if an object is under the mouse when it was moved.

  It triggers the events
*/
void QtOgre::checkObjectMouseMoved(QMouseEvent* event)
{
	// Check if there is any 2D object underneath.

	CEGUI::Window* targetWindow = has2dTargetAtPosition(event->pos());
	if(targetWindow != NULL)
	{
		// Window must be a ImageButton
		if(targetWindow->getType() != "AquaLook/ImageButton")
		{
			return;
		}
		if(targetWindow != _lastObject2dEntered)
		{
			_ogreWidget->setCursor(QCursor(Qt::PointingHandCursor));
		}
		// Check if there was an 3D object underneath before
		if( _lastObjectEntered != NULL )
		{
//			_ogreWidget->unsetCursor();
			_lastObjectEntered->onMouseLeaveEvent( event );
			_lastObjectEntered = NULL;
		}
	}
	else // If there isn't any 2D object below
	{
		QPointF p = event->pos();
		Object3DMousable* object = getObjectByMousePosition(p);
		if( _lastObjectEntered != object )
		{
			if(object != NULL)
			{
				_ogreWidget->setCursor(QCursor(Qt::PointingHandCursor));
//				QtCegui::setCeguiCursor("AquaLook", "MouseHand");
				object->onMouseMoveEvent(event);
				object->onMouseEnterEvent(event);
			}
			else
			{
				_ogreWidget->unsetCursor();
//				_ogreWidget->setCeguiCursor();
//				QtCegui::setCeguiCursor("AquaLook", "MouseArrow");
				_lastObjectEntered->onMouseLeaveEvent( event );
			}
			_lastObjectEntered = object;
		}
		else if( object != NULL )
		{
			object->onMouseMoveEvent(event);
		}

		// Check if there was an 2D object underneath before
		if( _lastObject2dEntered != NULL && object == NULL )
		{
			_ogreWidget->unsetCursor();
		}
	}
	_lastObject2dEntered = targetWindow;
}

/*
  Check if there are clickable 2D objects in the given position \a point.
*/
CEGUI::Window* QtOgre::has2dTargetAtPosition(const QPoint &point)
{
	CEGUI::Vector2 pos(point.x(), point.y());
	CEGUI::Window* guisheet = CEGUI::System::getSingleton().getGUISheet();
	if (guisheet)
	{
        CEGUI::Window* win = guisheet->getTargetChildAtPosition(pos);
        return win;
	}
	return false;
}

void QtOgre::createButtonMananger()
{
	if(_buttonsMgr == NULL)
	{
		_buttonsMgr = new Button2dManager(this);
	}
}

/*
+----------------------------------------------+
|             Getters / Setters                |
+----------------------------------------------+
*/

OSceneManager* QtOgre::oSceneManager_Ptr()
{
	return _oSceneLoaderMgr;
}

/*!
	Gets a pointer to OgreWidget
*/
OgreWidget *QtOgre::ogreWidget()
{
	return _ogreWidget;
}

/*!
  Returns the name of the light that is on (if many are one returns the first
*/
const QString QtOgre::lightOn()
{
	QString lightName;
	Ogre::Light *lt;
	QStringListIterator it(_lightList);
	while(it.hasNext())
	{
		lightName = it.next();
		lt = ogreWidget()->ogreSceneManager()->getLight(lightName.toStdString());
		if(lt->isVisible())
		{
			return lightName;
		}
	}
	return "";
}

/*!
  Lights the light named \a name.
*/
void QtOgre::setLightOn(const QString& name)
{
	qDebug() << "[Cnoti3dQtOgre::setLightOn]" << name;
	QString lightName;
	Ogre::Light *lt;
	QStringListIterator it(_lightList);
	while(it.hasNext())
	{
		lightName = it.next();
		lt = ogreWidget()->ogreSceneManager()->getLight(lightName.toStdString());
		if(lightName == name)
		{
			lt->setVisible(true);
		}
		else
		{
			lt->setVisible(false);
		}
	}
}

QStringList QtOgre::lightsOn()
{
	QStringList lightsList;
	QString lightName;
	Ogre::Light *lt;
	QStringListIterator it(_lightList);
	while(it.hasNext())
	{
		lightName = it.next();
		lt = ogreWidget()->ogreSceneManager()->getLight(lightName.toStdString());
		if(lt->isVisible())
		{
			lightsList << lightName;
		}
	}
	return lightsList;

}

void QtOgre::setLightsOn(const QStringList strList)
{
	qDebug() << "[Cnoti3dQtOgre::setLightsOn]" << strList;
	QString lightName;
	Ogre::Light *lt;
	QStringListIterator it(_lightList);
	while(it.hasNext())
	{
		lightName = it.next();
		lt = ogreWidget()->ogreSceneManager()->getLight(lightName.toStdString());
		if(strList.contains(lightName))
		{
			lt->setVisible(true);
		}
		else
		{
			lt->setVisible(false);
		}
	}
}

/*!
  Returns the light list.
*/
const QStringList QtOgre::lights()
{
	return _lightList;
}

void QtOgre::fadeIn( float delay )
{
	if(delay < 0.0f)
	{
		_fader->startFadeIn(_fadeTime);
	}
	else
	{
		_fader->startFadeIn(delay);
	}
}

void QtOgre::fadeOut( float delay )
{
	if(delay < 0.0f)
	{
		_fader->startFadeOut(_fadeTime);
	}
	else
	{
		_fader->startFadeOut(delay);
	}
}

} // namespace Cnoti3D
