#ifndef QTOGRE_OGREWIDGET_H_
#define QTOGRE_OGREWIDGET_H_

#include "Cnoti3d_global.h"
#include "interface/qtCegui.h"
#include "Camera.h"
// Ogre
#include <OgreCommon.h>
#include <OgrePrerequisites.h>
// Qt
#include <QSettings>
#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMoveEvent>

namespace Cnoti3D
{
	class ObjectManager;
	class Object3D;
	class Object3DMousable;
	class CameraManager;

	class CNOTI3DLIB_EXPORT OgreWidget : public QWidget, public QtCegui
	{
		Q_OBJECT
		Q_PROPERTY(QString activeCamera READ activeCameraName WRITE setActiveCamera)
	public:
		OgreWidget(QHash<QString, QString> options, QWidget* parent=0, Qt::WindowFlags f=0);
		~OgreWidget();

		//Other
		bool applySettings(QSettings* settings);
		void initialise(const Ogre::NameValuePairList *miscParams = 0);

		void setupResources();

		Object3DMousable* objectByMousePosition( const QPointF &mousePos, ObjectManager* objectMgr );

		Ogre::SceneManager* ogreSceneManager(){return _ogreSceneManager;}
		Ogre::RenderWindow* ogreRenderWindow(){return _ogreRenderWindow;}
        CEGUI::OgreRenderer* ceguiOgrerenderer();
		const QPointF &lastPosClicked(){return _lastPosClicked;}

		Camera* camera(const QString &name);
		Camera* activeCamera();
		QString activeCameraName();
		void setActiveCamera(const QString &name);
		void addCamera(const QString& name);

		void updateOgre(qreal timeSinceLastFrame);

		Object3DMousable * rayCastFromPoint(const Ogre::Ray &ray, ObjectManager *objectMgr);

	signals:
		void keyPressedEvent( QKeyEvent* event );
		void mouseReleasedEvent(QMouseEvent* event);
		void mouseMovedEvent(QMouseEvent*);
		void mousePressedEvent(QMouseEvent* event);
		void moved(QPoint p);
		void resized(QSize p);
		void closing();

	protected:
		void keyPressEvent(QKeyEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void resizeEvent(QResizeEvent* evt);
		void mousePressEvent(QMouseEvent *event);
		void moveEvent(QMoveEvent *event);
		void closeEvent(QCloseEvent * event);

	private:
		Ogre::Root *_ogreRoot;
		bool mIsInitialised;
		Ogre::NameValuePairList *_windowOptions;

		Ogre::SceneManager *_ogreSceneManager;
		Ogre::RenderWindow *_ogreRenderWindow;
		Ogre::RaySceneQuery* _rayQuery;	/// Scene query ray
		Ogre::Viewport* _ogreViewport;

		CameraManager* _cameraMgr;

		QPointF _lastPosClicked;

		void loadRenderSystemsFromPlugins(void);
	};
}

#endif /*QTOGRE_OGREWIDGET_H_*/
