#include "Balloon.h"
#include <Object3DAbstract>
#include <QOgreGraphicsView>
#include <Logger>
#include <Object3D>
#include <Object3DMousable>
#include <QOgreGLWidget>
#include <CameraManager>
#include <Camera>
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
#include <QSettings>
#include <QFile>
#include <QDataStream>

Balloon::Balloon( QObject* parent )
: Cnoti3D::Cnoti3dQtOgre( parent )
{
    qDebug()<<"Debug";
    qWarning()<<"Warning";
    qCritical()<<"Critical";

	initialise( 800, 600 );

	Ogre::Light* pointLight = qOgreGLWidget().ogreSceneManager()->createLight("pointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setPosition(Ogre::Vector3(0, 50, 500));

	pointLight->setDiffuseColour(1.0, 1.0, 1.0);
	pointLight->setSpecularColour(1.0, 1.0, 1.0);

	//loadScene("batutas.osm");

// LOAD UTF-8 TEXT FROM FILE
	// Get strings from file
	QString languageFile = "../media/moc-VD.txt";

	QFile file( languageFile );
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	in.setCodec("UTF-8");

	QStringList msgList;
	while (!in.atEnd()) {
		QString line = in.readLine();
		// Process line
		if( line.trimmed() != "" && line.indexOf( "=" ) != -1 )
		{
			QStringList strList = line.split("=");
			msgList.insert( strList[0].toInt() , strList[1] );
		}
	}

	file.close();
	QString txt = msgList.at(24);

// DRAW TEXT INTO PIXMAP
	QPixmap pix("../media/balloon_top.png");
	QPainter p;
	int strWidth = p.fontMetrics().width( txt );
	int strHeight = p.fontMetrics().height();

	int pixWidth = pix.width();
	int pixHeight = pix.height();

	p.begin( &pix );
	p.drawText( QRect( pixWidth / 2, pixHeight / 2, strWidth, strHeight), 0, txt );
	p.end();

	//QGraphicsPixmapItem* balloon = scene()->addPixmap(pix);
	pix.save("../media/balloon.png");

	qOgreGLWidget().makeOgreCurrent();

// TRANSFER IMAGE TO OGRE (//https://www.assembla.com/code/tofilovski-n900naali-fork/git/nodes/EntityComponents/EC_HoveringText/EC_HoveringText.cpp?rev=404af8fced71e892f70f8c0251303e9f62aee693)
	// Create texture
	QImage img = pix.toImage();
	Ogre::DataStreamPtr stream(new Ogre::MemoryDataStream((void*)img.bits(), img.byteCount()));
	Ogre::TextureManager &manager = Ogre::TextureManager::getSingleton();
	Ogre::Texture *tex = dynamic_cast<Ogre::Texture *>(manager.create("BalloonTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).get());
	assert(tex);

	tex->loadRawData(stream, img.width(), img.height(), Ogre::PF_A8R8G8B8);

	// Create a material using the texture
	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(
		"DynamicTextureMaterial", // name
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	mat->getTechnique(0)->getPass(0)->createTextureUnitState("BalloonTex");
	mat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

// SAVE TEXTURE TO IMAGE AND SAVE TO DISK
	// Declare buffer
	const size_t buffSize = tex->getSize();
	unsigned char *data = OGRE_ALLOC_T(unsigned char,buffSize,Ogre::MEMCATEGORY_GENERAL);

	// Clear buffer
	memset(data, 0, buffSize);

	// Setup Image with correct settings
	Ogre::Image i;
	i.loadDynamicImage(data, tex->getWidth(), tex->getHeight(), 1, Ogre::PF_R8G8B8A8, true);

	// Copy Texture buffer contents to image buffer
	Ogre::HardwarePixelBufferSharedPtr buf = tex->getBuffer();
	const Ogre::PixelBox destBox = i.getPixelBox();
	buf->blitToMemory(destBox);

	// Save to disk!
	i.save("../media/balloon_Ogre.png");

	qOgreGLWidget().makeCurrent();
// SHOW BALLOON
	//Create BillboardSet
	Ogre::BillboardSet* _bbSet = qOgreGLWidget().ogreSceneManager()->createBillboardSet("BalloonSSet", 1);
	_bbSet->setDefaultDimensions( 180, 90 );
	_bbSet->setVisible( true );

	// Create Billboard
	Ogre::Billboard* _bb = _bbSet->createBillboard( 180, 90, 0 );
	_bb->setPosition(0,50,300);


	Ogre::SceneNode* bbNode = this->qOgreGLWidget().ogreSceneManager()->getRootSceneNode()->createChildSceneNode();
	bbNode->attachObject( _bbSet );

	_bbSet->setMaterialName(mat->getName());

	// SHOW GRAPHICS VIEW
	_gView->show();

}

Balloon::~Balloon()
{
	shutdown();
}
