#ifndef CAMERA_H
#define CAMERA_H

#include "Cnoti3d_global.h"
// Ogre
#include <Ogre.h>
// Qt
#include <QObject>
#include <QString>
#include <QVector3D>
#include <QTimerEvent>

class QPropertyAnimation;

namespace Cnoti3D {

class CNOTI3DLIB_EXPORT Camera: public QObject
{
	Q_OBJECT
	Q_PROPERTY(QVector3D position READ getPosition WRITE setPosition)
	Q_PROPERTY(QVector3D targetPosition READ getTargetPosition WRITE setTarget)
public:
	Camera(Ogre::SceneManager *sceneMgr, const QString& name);
	Camera(Ogre::SceneManager *sceneMgr, const QString& name, QVector3D &pos, QVector3D &target);

	void moveTo(float time = 1000);
    void moveTo(QVector3D newPos, QVector3D newTargetPos, float time = 1000);
	void stopMoving();

	//
	// Set
	//
	void setAspectRatio( qreal x, qreal y);
	void setPosition( float x, float y, float z );
	void setPosition( QVector3D pos );
	void setTarget( float x, float y, float z );
	void setTarget( QVector3D pos );

	//
	// Get
	//
	QString getName();
	Ogre::SceneNode* getNode();
	Ogre::SceneNode* getTargetNode();
	Ogre::Camera* getCamera();
	Ogre::Ray getCameraToViewportRay( float x, float y );
	QVector3D getPosition();
	QVector3D getTargetPosition(){return _targetPosition;}
	bool isZoomed(){return _hasZoomApplied;}

public slots:
    void zoomInToObject(Ogre::SceneNode* sceneNode, float time);
    void zoomOff(float time, int wait = 0);

signals:
	void stoppedMoving();

protected:
	Ogre::SceneNode* _targetNode;	/// The camera target
	Ogre::SceneNode* _cameraNode;

	Ogre::Camera* _camera;	/// Camera object

	QVector3D _position;		/// Camera current position
	QVector3D _defaultPosition;	/// Camera default position (used for zoom, etc.)
	QVector3D _targetPosition;
	QVector3D _defaultTargetPosition;
	QPropertyAnimation* _animation;
	QPropertyAnimation* _targetAnimation;

	bool _hasZoomApplied;
    int  _zoomOffTimerId;
	int  _zoomTime;

	// Functions
	void init( Ogre::SceneManager *sceneMgr, const QString& name, QVector3D &pos, QVector3D &target );
	void timerEvent(QTimerEvent *event);

private slots:
	void stopMovingTasks();
};

}	// namespace Cnoti3D
#endif // CAMERA_H
