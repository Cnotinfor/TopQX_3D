#include "OgreWidget.h"
#include "cameramanager.h"
#include "object3dmousable.h"
#include "ObjectManager.h"
#include "OgreUtils.h"
#include "settings/Cnoti3dSettings.h"
// Ogre
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreStringConverter.h>
#include <OgreEntity.h>
#include <OgreConfigFile.h>
// Qt
#include <QDebug>
// CEGUI
#include <CEGUIWindow.h>

#if defined(Q_WS_X11)
#include <QX11Info>
#endif

namespace Cnoti3D
{
	OgreWidget::OgreWidget(QHash<QString, QString> options, QWidget* parent, Qt::WindowFlags f)
	:QWidget(parent, f | Qt::MSWindowsOwnDC)
	,QtCegui()
	,_ogreRenderWindow(NULL)
	,mIsInitialised(false)
	,_ogreSceneManager(NULL)
	,_rayQuery(NULL)
	{
		QPalette palette = this->palette(); //FIXME - Doesn't compile unless we use 'this'?! Because we are in the constructor?
		palette.setColor(QPalette::Active, QPalette::WindowText, Qt::black);
		palette.setColor(QPalette::Active, QPalette::Window, Qt::black);
		setPalette(palette);

		// Must know where mouse is to send to CEGUI
		setMouseTracking(true);

		//Create the root and load render system plugins. We do that here so that we know
		//what render systems are available by the time we show the settings dialog.
		//Note that the render system is not initialised until the user selects one.
		_ogreRoot = new Ogre::Root("");
#ifndef Q_WS_WIN
		_ogreRoot->loadPlugin("RenderSystem_GL");
#else
	#ifdef QT_DEBUG
		_ogreRoot->loadPlugin("RenderSystem_GL_d");
		_ogreRoot->loadPlugin("RenderSystem_Direct3D9_d");
	#else
		_ogreRoot->loadPlugin("RenderSystem_GL");
		_ogreRoot->loadPlugin("RenderSystem_Direct3D9");
	#endif
#endif
		// get the render system from the options
		QString renderSystemName = options.value("Render System", "undefined");
		Ogre::RenderSystem *renderSystem = NULL;

		if(renderSystemName == "undefined")
		{
			//if undefined get the first render system
			renderSystem = *_ogreRoot->getAvailableRenderers().begin();
		}
		else
		{
			const Ogre::RenderSystemList& renderSystemList = _ogreRoot->getAvailableRenderers();
			for (size_t i = 0 ; i < renderSystemList.size() ; i++)
			{
				if(renderSystemName.compare( QString::fromStdString( renderSystemList[i]->getName() ) ) == 0)
				{
					renderSystem = renderSystemList[i];
				}
			}

			// remover the Render System from the options, since it is not valid option to be applied
			options.remove("Render System");
		}

		qDebug() << "[OgreWidget::OgreWidget]" << options;

		// Set the options for the render system
		// Create a value pair list of options to use by the render system
		// The options are Hash<QString,Qstring> and must be converted to NameValuePairList
		_windowOptions = new Ogre::NameValuePairList();
		QHashIterator<QString, QString> it(options);
		while(it.hasNext())
		{
			it.next();

			if(it.key() != Settings::RENDER_FULLSCREEN)
			{
				//renderSystem->setConfigOption( it.key().toStdString(), it.value().toStdString() );
				(*_windowOptions)[it.key().toStdString()] = it.value().toStdString();
				qDebug() << "[OgreWidget::OgreWidget]" << it.key() << ":" << it.value();
			}
		}

		_ogreRoot->setRenderSystem( renderSystem );

		// Camera manager
		_cameraMgr = CameraManager::instance();

		_ogreRoot->initialise(false);
	}

	OgreWidget::~OgreWidget()
	{
	}

	void OgreWidget::initialise(const Ogre::NameValuePairList *miscParams)
	{
		//These attributes are the same as those use in a QGLWidget
		setAttribute(Qt::WA_PaintOnScreen);
		setAttribute(Qt::WA_NoSystemBackground);

		//Parameters to pass to Ogre::Root::createRenderWindow()
		Ogre::NameValuePairList params;

		//If the user passed in any parameters then be sure to copy them into our own parameter set.
		//NOTE: Many of the parameters the user can supply (left, top, border, etc) will be ignored
		//as they are overridden by Qt. Some are still useful (such as FSAA).
		if(miscParams != 0)
		{
			params.insert(miscParams->begin(), miscParams->end());
		}
		else
		{
			// It uses the options that were used in the constructor
			params.insert(_windowOptions->begin(), _windowOptions->end());
		}

		//The external windows handle parameters are platform-specific
		Ogre::String externalWindowHandleParams;

		//Accept input focus
		setFocusPolicy(Qt::StrongFocus);

	#if defined(Q_WS_WIN)
		//positive integer for W32 (HWND handle) - According to Ogre Docs
		externalWindowHandleParams = Ogre::StringConverter::toString((unsigned int)(winId()));
	#endif

#if defined(Q_WS_MAC)
		externalWindowHandleParams = Ogre::StringConverter::toString((size_t) (this->winId()));
#endif

	#if defined(Q_WS_X11)
		//poslong:posint:poslong:poslong (display*:screen:windowHandle:XVisualInfo*) for GLX - According to Ogre Docs
		QX11Info info = x11Info();
		externalWindowHandleParams  = Ogre::StringConverter::toString((unsigned long)(info.display()));
		externalWindowHandleParams += ":";
		externalWindowHandleParams += Ogre::StringConverter::toString((unsigned int)(info.screen()));
		externalWindowHandleParams += ":";
		externalWindowHandleParams += Ogre::StringConverter::toString((unsigned long)(winId()));
		//externalWindowHandleParams += ":";
		//externalWindowHandleParams += Ogre::StringConverter::toString((unsigned long)(info.visual()));
	#endif

		//Add the external window handle parameters to the existing params set.
	#if defined(Q_WS_WIN)
		params["externalWindowHandle"] = externalWindowHandleParams;
	#endif

	#if defined(Q_WS_MAC)
		params["externalWindowHandle"] = externalWindowHandleParams;

		params["macAPI"] = "cocoa";
		params["macAPICocoaUseNSView"] = "true";
	#endif

	#if defined(Q_WS_X11)
		params["parentWindowHandle"] = externalWindowHandleParams;
	#endif

		//Finally create our window.
		_ogreRenderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("OgreWindow", width(), height(), false, &params);

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		_ogreRenderWindow->windowMovedOrResized();
		//resizeEvent(0);
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		_ogreRenderWindow->resize(width(), height());
#endif

		// Create Ogre scene manager
		_ogreSceneManager = _ogreRoot->createSceneManager( Ogre::ST_GENERIC, "SceneManager" + Ogre::StringConverter::toString((unsigned long)this) );

		// Creates ray query object
		_rayQuery = _ogreSceneManager->createRayQuery( Ogre::Ray(), Ogre::SceneManager::ENTITY_TYPE_MASK );
		if( _rayQuery != NULL )
		{
			// Max of 10 objects to include in the query results
			// Objects will be sorted by distance
			_rayQuery->setSortByDistance( true, 10 );
		}
		else
		{
			qWarning() << "[QOgreGLWidget::initializeGL] Problem initializing rayQuery.";
		}

		// Create a default camera
		_cameraMgr->addCamera(_ogreSceneManager,"defaultCamera");

		// Create viewport
		_ogreViewport = _ogreRenderWindow->addViewport(_cameraMgr->camera("defaultCamera")->getCamera());
		_ogreViewport->setBackgroundColour( Ogre::ColourValue( 0.0f,0.0f,0.3f ) );

		// Activate default camera
		_cameraMgr->setActiveCamera("defaultCamera", _ogreViewport);
		_cameraMgr->activeCamera()->setAspectRatio( (Ogre::Real) _ogreRenderWindow->getWidth(), _ogreRenderWindow->getHeight() );

		// Ambient Light
		_ogreSceneManager->setAmbientLight( Ogre::ColourValue( 1, 1, 1 ) );

//		initialiseGui(_ogreRenderWindow);

		mIsInitialised = true;
	}

	void OgreWidget::updateOgre(qreal timeSinceLastFrame)
	{
		if(mIsInitialised)
		{
			updateGui(timeSinceLastFrame);
			Ogre::Root::getSingleton().renderOneFrame();
		}
	}

	void OgreWidget::resizeEvent(QResizeEvent* evt)
	{
		qDebug() << "[OgreWidget::resizeEvent] width:" << width() << " height:" << height();
		if(_ogreRenderWindow)
		{
			_ogreRenderWindow->resize(width(), height());
			_ogreRenderWindow->windowMovedOrResized();


			CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Size(size().width(), size().height()));

			// Redefine the CEGUI Size
			//CEGUI::Window* guiSheet = CEGUI::System::getSingleton().getGUISheet();
//			if( guiSheet != NULL )
//			{

//				//guiSheet->setSize(CEGUI::UVector2(CEGUI::UDim(0, size().width()), CEGUI::UDim(0, size().height())));
//				//qDebug() << "[OgreWidget::resizeEvent]" << size();
//			}
		}
		// Redefine the aspect ratio
		if(_cameraMgr->activeCamera())
		{
			_cameraMgr->activeCamera()->setAspectRatio( width(), height() );
		}
		if(isVisible())
		{
			emit resized(size());
		}
	}

	bool OgreWidget::applySettings(QSettings* settings)
	{
		bool applied = false;
		QString resolution;

		QStringList windowModes = settings->value("Graphics/WindowModes").toStringList();
		int selectedWindowMode = settings->value("Graphics/SelectedWindowMode", 0).toInt();

		if(windowModes.size() > selectedWindowMode) //Make sure it's a valid index.
		{
			resolution = windowModes.at(selectedWindowMode);
			if(resolution.compare("FullScreen", Qt::CaseInsensitive) == 0)
			{
				showFullScreen();
				applied = true;
			}
			else
			{
				QStringList splitResolution = resolution.split("x", QString::SkipEmptyParts, Qt::CaseInsensitive);

				if(splitResolution.size() == 2)
				{
					bool widthValid = false;
					int width = splitResolution.at(0).toInt(&widthValid);
					widthValid &= (width > 100) && (width < 10000);

					bool heightValid = false;
					int height = splitResolution.at(1).toInt(&heightValid);
					heightValid &= (width > 100) && (width < 10000);

					if(widthValid && heightValid)
					{
						resize(width, height);
//						Application::centerWidget(this);
						applied = true;
					}
				}
			}
		}

//		if(applied)
//		{
//			qApp->_systemLog()->logMessage("Set main window resolution to \"" + resolution + "\"", LL_INFO);
//		}
//		else
//		{
//			qApp->_systemLog()->logMessage("Failed to set main window resolution. The list of resolutions was:", LL_WARNING);
//			foreach (QString res, windowModes)
//			{
//				qApp->_systemLog()->logMessage("    \"" + res + "\"", LL_WARNING);
//			}
//			qApp->_systemLog()->logMessage("The requested index was: \"" + QString::number(selectedWindowMode) + "\"", LL_WARNING);
//		}
		return applied;
	}

	void OgreWidget::setupResources()
	{
		//
		// Load resource paths from config file
		//
		Ogre::ConfigFile cf;

#ifdef Q_WS_MAC
		QString appPath = QApplication::applicationDirPath(); // Without this it doesn't work for Mac.
#ifdef QT_DEBUG
		cf.load(appPath.toStdString() + "/resources_d.cfg");
#else
		cf.load(appPath.toStdString() + "/resources.cfg");
#endif
#else
		// For windows don't use full path, if a path has strange characters it will not work
#ifdef QT_DEBUG
		cf.load("resources_d.cfg");
#else
		cf.load("resources.cfg");
#endif
#endif
		//
		// Go through all sections & settings in the file
		//
		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

		Ogre::String secName, typeName, archName;
		while(seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for(i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;
#ifdef Q_WS_MAC
				//
				// OS X does not set the working directory relative to the app,
				// In order to make things portable on OS X we need to provide
				// the loading with it's own bundle path location
				//
//				if(!StringUtil::startsWith(archName, "/", false))
//				{
//					//
//					// only adjust relative dirs
//					//
//					archName = String(macBundlePath() + "/" + archName);
//				}
#endif
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
			}
		}
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}

	/*!
	Casts a ray from mouse position into the scene to get the first object that is mouse aware.
	*/
	Object3DMousable* OgreWidget::rayCastFromPoint( const Ogre::Ray &ray, ObjectManager* objectMgr )
	{
		// Initialization of rayQuery, only needed once
		if( _rayQuery == NULL )
		{
			return NULL;
		}

		// Clear previous execution
		_rayQuery->clearResults();
		// set the new ray to query
		_rayQuery->setRay( ray );
		// execute query. returns a vector of hits
		if( _rayQuery->execute().size() <= 0 )
		{
			return NULL;
		}


		Object3DMousable* objToReturn = NULL;
		// at this point we have raycast to a series of different objects bounding boxes.
		// we need to test these different objects to see which is the first polygon hit.
		// there are some minor optimizations (distance based) that mean we wont have to
		// check all of the objects most of the time, but the worst case scenario is that
		// we need to test every triangle of every object.
		Ogre::Real closest_distance = 99999.0f;
		Ogre::Vector3 closest_result;
		Ogre::RaySceneQueryResult &query_result = _rayQuery->getLastResults();
		//CnotiLogManager::getSingleton().logMessage( "RayCastFromPoint --- query_result size:" + Ogre::StringConverter::toString( query_result.size() ) );
		for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
		{
			if( closest_distance < query_result[qr_idx].distance)
			{
				break; // Stop the for, since
			}

			// only check this result if its a hit against an entity
			if( (query_result[qr_idx].movable != NULL) && (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0) )
			{
				// get the entity to check
				Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);

				//	Go to next if entity is no visible
				if( !pentity->isVisible() )
				{
					continue;
				}

				Object3DMousable* object = objectMgr->isObjectMouseAwareAvailable(pentity->getName().c_str());
				// test if it is mouse aware
				if(object)
				{
					bool new_closest_found = false;

					if(object->isBoundBoxClickable())
					{
//						return object;
						new_closest_found = true;
						// TODO: Check if is the closest object
//						QVector3D v = object->position();
//						std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
//							vertices[indices[i+1]], vertices[indices[i+2]], true, false);
					}
					else
					{
						// mesh data to retrieve
						size_t vertex_count;
						size_t index_count;
						Ogre::Vector3 *vertices;
						unsigned long *indices;

						// get the mesh information
						OgreUtils::getMeshInformation( pentity->getMesh(), vertex_count, vertices, index_count, indices,
													   pentity->getParentSceneNode()->_getDerivedPosition(),
													   pentity->getParentSceneNode()->_getDerivedOrientation(),
													   pentity->getParentSceneNode()->getScale() );

						// test for hitting individual triangles on the mesh
						for (int i = 0; i < static_cast<int>(index_count); i += 3)
						{
							// check for a hit against this triangle
							std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
								vertices[indices[i+1]], vertices[indices[i+2]], true, false);

							// if it was a hit check if its the closest
							if (hit.first)
							{
								if ((closest_distance < 0.0f) ||
									(hit.second < closest_distance))
								{
									// this is the closest so far, save it off
									closest_distance = hit.second;
									new_closest_found = true;
								}
							}
						}

						// free the verticies and indicies memory
						delete[] vertices;
						delete[] indices;
					}
					// if we found a new closest raycast for this object, update the
					// closest_result before moving on to the next object.
					if (new_closest_found)
					{
						closest_result = ray.getPoint(closest_distance);
						objToReturn = object;
					}
				}
			}
		}

		// clear query results for current query
		_rayQuery->clearResults();

		return objToReturn;

	}

	Cnoti3D::Object3DMousable * Cnoti3D::OgreWidget::objectByMousePosition(const QPointF &mousePos, Cnoti3D::ObjectManager *objectMgr)
	{
		// make sure we get coords in [0..1]
		float x, y;
		x = mousePos.x() / (float) width();
		y = mousePos.y() / (float) height();

		Ogre::Ray mouseRay = _cameraMgr->activeCamera()->getCameraToViewportRay(x,y);

		// get point in world
		Ogre::Vector3 res;
		Ogre::String str;
		Object3DMousable* hit = rayCastFromPoint( mouseRay, objectMgr );

        return hit;
    }

    CEGUI::OgreRenderer *OgreWidget::ceguiOgrerenderer()
    {
        return QtCegui::ceguiRenderer();
    }

	/*
	+----------------------------------------------+
	|             Camera management                |
	+----------------------------------------------+
	*/

	Camera* OgreWidget::camera(const QString &name)
	{
		return _cameraMgr->camera(name);
	}

	Camera* OgreWidget::activeCamera()
	{
		return _cameraMgr->activeCamera();
	}

	QString OgreWidget::activeCameraName()
	{
		return _cameraMgr->activeCamera()->getName();
	}

	void OgreWidget::setActiveCamera(const QString &name)
	{
		_cameraMgr->setActiveCamera(name, _ogreViewport);
	}

	void OgreWidget::addCamera(const QString& name)
	{
		_cameraMgr->addCamera(_ogreSceneManager, name);
	}

	/*
	+----------------------------------------------+
	|                 Keyboard                     |
	+----------------------------------------------+
	*/

	void OgreWidget::keyPressEvent (QKeyEvent* event)
	{
		// CEGUI
//		bool processed = (event->text ().size () == 1) && CEGUI::System::getSingleton ().injectChar (event->text ().at (0).unicode ());
//		if (!processed)
//			processed = processed || CEGUI::System::getSingleton ().injectKeyDown (convertQtToCEGUIKey (event->key (), event->modifiers ()));
		emit keyPressedEvent(event);
	}

	/*
	+----------------------------------------------+
	|                   Mouse                      |
	+----------------------------------------------+
	*/

	void OgreWidget::mouseReleaseEvent ( QMouseEvent * event )
    {
        // Save position clicked
        _lastPosClicked = event->posF();
        // Send event for 3D handling
        event->setAccepted(false);
        emit mouseReleasedEvent(event);
        if (!event->isAccepted())
        {
            // Send event for 2D handling
            CEGUI::System::getSingleton ().injectMouseButtonUp (convertQtToCEGUIMouseButton (event->button ()));
            event->accept();
        }
	}

	void OgreWidget::mouseMoveEvent ( QMouseEvent * event )
    {
        emit mouseMovedEvent(event);
		float mouseXPos = event->posF().x();
		float mouseYPos = event->posF().y();
        CEGUI::System::getSingleton ().injectMousePosition (mouseXPos, mouseYPos);
	}

	/*!
	Reimplemented from QGraphicsView::mouseReleaseEvent(QMouseEvent* event)
	*/
	void OgreWidget::mousePressEvent(QMouseEvent* event)
	{
//		emit mousePressedEvent(event);
		CEGUI::System::getSingleton ().injectMouseButtonDown (convertQtToCEGUIMouseButton (event->button ()));
	}

	void OgreWidget::moveEvent(QMoveEvent *event)
	{
		if(isVisible())
		{
			emit moved(pos());
		}
	}

	void OgreWidget::closeEvent(QCloseEvent *event)
	{
		emit closing();
		event->ignore();
		//QWidget::closeEvent(event);
	}
}
