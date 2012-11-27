#if !defined(_CNOTI3DQTOGRE_H)
#define _CNOTI3DQTOGRE_H

#include "Cnoti3d.h"
#include "Cnoti3d_global.h"
// Qt
#include <QObject>
#include <QPointer>
#include <QHash>
#include <QPoint>
#include <QSize>
#include <QStringList>
#include <QTime>
#include <QMutex>
// Ogre
#include <Ogre.h>

// Qt
class QTimer;
class QPointF;
class QGraphicsView;
class QMouseEvent;
class QHelpEvent;

namespace CEGUI {
	class Window;
}

namespace Cnoti3D {

class OgreWidget;
class QOgreGraphicsScene;
class QOgreGraphicsView;
class Object3DAbstract;
class Object3DMousable;
class OSceneManager;
class Button2dManager;
class StateManager;
class State;
class Fader;


class CNOTI3DLIB_EXPORT QtOgre: public QObject, public Cnoti3d
{
	Q_OBJECT
	Q_PROPERTY(QString turnLightOn READ lightOn WRITE setLightOn)
	Q_PROPERTY(QStringList turnLightsOn READ lightsOn WRITE setLightsOn)

	public:
		friend class State;

		QtOgre( QObject *parent = 0 );
		~QtOgre();

		void initialise( QHash<QString, QString> options = (QHash<QString, QString>()) );
		void shutdown();

		void resize( int width, int height );

		void loadScene(const QString& filename);
		Object3DAbstract* createObject(QString entityName);
		Object3DAbstract* createObject(QString objectName, QString meshFile, QString animationFile = "", Clickable clickable = notClickable);
		Object3DAbstract* createObject(QString objectName, QString meshFile, QString animationFile, qreal x, qreal y, qreal z, Clickable clickable = notClickable);
		Object3DAbstract* createObject(QString objectName, QString meshFile, qreal x, qreal y, qreal z, Clickable clickable = notClickable);
		Object3DAbstract* addObject(Object3DAbstract *object);

		Ogre::Entity* createEntity(const QString& objectName, const QString& meshFile);

		void loadResource(QString resourceFile);
		void unloadResource(QString resourceFile);

		void addState(State *state);
		void setInitialState(State *state);
		void startStates();

		Object3DMousable *getObjectByMousePosition( const QPointF &mousePos );

		Ogre::Entity* entity(const QString &objectName);

		OSceneManager *oSceneManager_Ptr();
		Object3DAbstract *object( const QString &objectName );

		OgreWidget *ogreWidget();

		void setFadeTime(float time){_fadeTime = time;}
		float fadeTime(){return _fadeTime;}

		bool isFullScreen(){return _fullscreen;}


public slots:
		virtual void update();
		virtual void widgetMoved(QPoint p);
		virtual void widgetResized(QSize s);
		void checkObjectMouseMoved(QMouseEvent *event);
		void checkObjectMouseReleased(QMouseEvent *event);
		void fadeIn( float delay = -1.0f );
		void fadeOut( float delay = -1.0f );

	signals:
		void ObjectClicked(Object3DMousable *object);


	protected:
		// Methods
		void createButtonMananger();

	protected:
		StateManager *_stateMgr;
		// Methods
		const QString lightOn();
		void setLightOn(const QString& name);
		QStringList lightsOn();
		void setLightsOn(QStringList strList);
		const QStringList lights();

	private:
		OgreWidget *_ogreWidget;
		QTimer *_timer;
		QTime _frameTimer;	// To obtain the time that decurres between updates (frames);
		QMutex _loadMutex;
		OSceneManager* _oSceneLoaderMgr;

		Object3DMousable *_lastObjectEntered;
		CEGUI::Window *_lastObject2dEntered;
		Button2dManager  *_buttonsMgr;

		QStringList _lightList;

		float _fadeTime;
		Fader* _fader;

		bool _fullscreen;

		// Methods
		CEGUI::Window* has2dTargetAtPosition(const QPoint &point);
};

}

#endif  //_CNOTI3DQTOGRE_H
