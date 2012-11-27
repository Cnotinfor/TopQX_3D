#include "cegui.h"
#include "CeguiState.h"
// CEGUI
#include <CEGUIWindowManager.h>
#include <CEGUIResourceProvider.h>
#include <CEGUIDefaultResourceProvider.h>
#include <CEGUIImageset.h>
#include <CEGUIFont.h>
#include <CEGUISchemeManager.h>
#include <CEGUIEvent.h>
#include <elements/CEGUIFrameWindow.h>
#include <elements/CEGUIPushButton.h>
// Cnoti3D
#include <OgreWidget>
#include <Button2d>
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

CeguiExample::CeguiExample( QObject* parent )
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

	resources();

//	// Load the scheme
//	CEGUI::SchemeManager::getSingleton().create( "TaharezLook.scheme" );

//	// Set the defaults
//	CEGUI::System::getSingleton().setDefaultFont( "DejaVuSans-10" );
//		CEGUI::System::getSingleton().setDefaultMouseCursor( "TaharezLook", "MouseArrow" );

//	usingCEGUI();

	usingCnoti3D();

	ogreWidget()->show();
}

CeguiExample::~CeguiExample()
{
	shutdown();
}

bool CeguiExample::onLeftClick(const CEGUI::EventArgs& pEventArgs)
{
	qDebug() << "[CeguiExample::onLeftClick]";

	return true;
}

void CeguiExample::usingCnoti3D()
{
	_state = new CeguiState(this);
	addState(_state);
	setInitialState( _state );
	startStates();
}

void CeguiExample::usingCEGUI()
{
	// All windows and widgets are created via the WindowManager singleton.
	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	// rootWindow
	CEGUI::Window* myRoot = winMgr.createWindow( "DefaultWindow", "_MasterRoot" );
	CEGUI::System::getSingleton().setGUISheet( myRoot );

	// Create a FrameWindow in the TaharezLook style, and name it 'Demo Window'
	CEGUI::FrameWindow* wnd = (CEGUI::FrameWindow*)winMgr.createWindow("AquaLook/FrameWindow", "Demo Window");

	// Here we attach the newly created FrameWindow to the previously created
	// DefaultWindow which we will be using as the root of the displayed gui.
	myRoot->addChildWindow(wnd);

	// Here we set the FrameWindow so that it is half the size of the display,
	// and centered within the display.
	wnd->setPosition(CEGUI::UVector2(cegui_reldim(0.40f), cegui_reldim( 0.05f)));
	wnd->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.5f)));

	// here we set a maximum size for the FrameWindow which is equal to the size
	// of the display, and a minimum size of one tenth of the display.
	wnd->setMaxSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
	wnd->setMinSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.1f)));

	// As a final step in the initialisation of our sample window, we set the window's
	// text to "Hello World!", so that this text will appear as the caption in the
	// FrameWindow's titlebar.
	wnd->setText("Hello World!");

//	CEGUI::PushButton* pushButton = static_cast<CEGUI::PushButton*>(winMgr.createWindow("TaharezLook/Button", "Pushbutton"));
//	myRoot->addChildWindow(pushButton);
//	pushButton->setPosition(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.10f)));
//	pushButton->setSize(CEGUI::UVector2(cegui_reldim(0.15f), cegui_reldim( 0.05f)));
//	pushButton->setText("Click");
//	pushButton->setTooltipText("Click here...");
////	pushButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&CeguiExample::onRightClick,	this));
//	pushButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CeguiExample::onLeftClick,		this));



	//	CEGUI::Imageset &ButtonsImageset = CEGUI::ImagesetManager::getSingleton().createFromImageFile("Buttons", "balloon_top.png");
	//	(&ButtonsImageset)->defineImage("ButtonUp", CEGUI::Point(0.0f,0.0f), CEGUI::Size( 0.5f, 0.5f ), CEGUI::Point(0.0f,0.0f));
	//	(&ButtonsImageset)->defineImage("ButtonDown", CEGUI::Point(0.0f,0.5f), CEGUI::Size( 0.5f, 0.5f ), CEGUI::Point(0.0f,0.0f));

	//	CEGUI::PushButton* NewGame = (CEGUI::PushButton*)winMgr.createWindow("TaharezLook/ImageButton", "NewButton");
	//	CEGUI::PushButton* NewGame = new CEGUI::PushButton("TaharezLook/ImageButton", "NewButton");
	//	myRoot->addChildWindow( NewGame );
	//	NewGame->setPosition( CEGUI::UVector2( CEGUI::UDim( 0.2f, 0.0f ), CEGUI::UDim( 0.2f, 0.0f ) ) );
	//	NewGame->setSize( CEGUI::UVector2( CEGUI::UDim( 0.2f, 0.0f ), CEGUI::UDim( 0.1f, 0.0f ) ) );
	//	NewGame->setText("New Game");
	//	NewGame->setProperty( "NormalImage", "set:Buttons image:full_image" );
	////	NewGame->setProperty( "NormalImage", "set:Buttons image:ButtonUp" );
	////	NewGame->setProperty( "HoverImage", "set:Buttons image:ButtonDown" );
	////	NewGame->setProperty( "PushedImage", "set:Buttons image:ButtonDown" );
}

void CeguiExample::resources()
{
	// Moved to resources.cfg file

//	//Resources
//	Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();

//	// add resource groups that we use
//	rgm.createResourceGroup("imagesets");
//	rgm.createResourceGroup("fonts");
//	rgm.createResourceGroup("layouts");
//	rgm.createResourceGroup("schemes");
//	rgm.createResourceGroup("looknfeels");
//	rgm.createResourceGroup("lua_scripts");
//	rgm.createResourceGroup("schemas");

//	// add CEGUI sample framework datafile dirs as resource locations
////		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem");

//	const char* dataPathPrefix = "../Resources/gui/";
//	char resourcePath[1024];

//	// for each resource type, set a resource group directory
//	sprintf(resourcePath, "%s/%s", dataPathPrefix, "schemes/");
//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "schemes");
//	sprintf(resourcePath, "%s/%s", dataPathPrefix, "imagesets/");
//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "imagesets");
//	sprintf(resourcePath, "%s/%s", dataPathPrefix, "fonts/");
//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "fonts");
//	sprintf(resourcePath, "%s/%s", dataPathPrefix, "layouts/");
//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "layouts");
//	sprintf(resourcePath, "%s/%s", dataPathPrefix, "looknfeel/");
//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "looknfeels");
//	sprintf(resourcePath, "%s/%s", dataPathPrefix, "lua_scripts/");
//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "lua_scripts");
//	sprintf(resourcePath, "%s/%s", dataPathPrefix, "xml_schemas/");
//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "schemas");
}
