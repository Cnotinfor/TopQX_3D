#if !defined(_OBJECT3DABSTRACT_H)
#define _OBJECT3DABSTRACT_H

#include "Cnoti3d_global.h"
// Qt
#include <QVector3D>
#include <QObject>

namespace Cnoti3D {

	class CNOTI3DLIB_EXPORT Object3DAbstract: public QObject
	{
		Q_OBJECT
		Q_PROPERTY( QVector3D position READ position WRITE setPosition )
	public:
		Object3DAbstract(QObject* parent=0);
		~Object3DAbstract();
		//virtual void setPosition( qreal x, qreal y, qreal z );
		virtual void setPosition( const QVector3D& pos );
		virtual QVector3D position()const;
		virtual qreal xPos()const;
		virtual qreal yPos()const;
		virtual qreal zPos()const;
		virtual void update( float timeSinceLastFrame ) = 0;
		virtual bool isVisible()const = 0;
		virtual void show() = 0;
		virtual void hide() = 0;
		virtual void setOrientation( const QVector3D& lookTo ) = 0;
		virtual QString name()const = 0;
	protected:
		QVector3D _position;

	};

}	// namespace Cnoti3D

#endif  //_OBJECT3DABSTRACT_H
