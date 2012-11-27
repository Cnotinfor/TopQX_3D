#ifndef OBJECT3DMOUSABLE_H
#define OBJECT3DMOUSABLE_H

#include "Cnoti3d_global.h"
#include "Object3d.h"

#include <Ogre.h>

class QMouseEvent;
class QWheelEvent;

namespace Cnoti3D
{
	class CNOTI3DLIB_EXPORT Object3DMousable : public Object3D
	{
		Q_OBJECT
		Q_PROPERTY(bool enable READ isEnable WRITE setEnable )
		Q_PROPERTY(int angle READ angle WRITE setAngle )
		Q_PROPERTY(QString tooltip READ tooltip WRITE setTooltip )

	public:
		Object3DMousable( Ogre::SceneManager* sceneManager, const QString& objectName );
		Object3DMousable( Ogre::Entity* entity );

		void addNode(const QString& name);

		const bool isEnable() {return _enable;}
		void setEnable(bool status) {_enable = status;}

		const bool rotateOnMouseHover() {return _rotateOnMouseHover;}
		void setRotateOnMouseHover(bool rotate) {_rotateOnMouseHover = rotate;}

		void setTooltip(QString text){_tooltipText = text;}
		QString tooltip(){ return _tooltipText; }

		void mouseLeave();

		const int angle(){return _angle;}
		void setAngle(int angle);

		void setBoundBoxClickable(bool value);
		bool isBoundBoxClickable();

	public slots:
		virtual void onMouseDoubleClickEvent ( QMouseEvent * mouseEvent );
		virtual void onMouseMoveEvent ( QMouseEvent * mouseEvent );
		virtual void onMousePressEvent ( QMouseEvent * mouseEvent );
		virtual void onMouseReleaseEvent ( QMouseEvent * mouseEvent );
		virtual void onWheelEvent ( QWheelEvent * wheelEvent );
		virtual void onMouseEnterEvent( QMouseEvent* mouseEvent );
		virtual void onMouseLeaveEvent( QMouseEvent* mouseEvent );

	signals:
		void clicked();

	protected:
		void timerEvent(QTimerEvent *event);

	private:
		bool _enable;
		bool _boundBoxClickable;
		//Tooltip
		int     _tooltipTimer;
		QPoint  _tooltipPos;
		QString _tooltipText;

		// Variable to handle rotate animation on mouse hover
		int  _angle;
		bool _rotateOnMouseHover;
		int  _animationTimer;

		// Functions
		void init();
		void startRotation();
		void stopRotation();
	};
}
#endif // OBJECT3DMOUSABLE_H
