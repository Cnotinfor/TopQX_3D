/*!
\class Object3DAbstract Object3dAbstract.h <Object3DAbstract>

The Object3DAbstract class is the base class of Object3D.

The Object3DAbstract class is the abstract base class of Object3D. It defines the interface that all 3d objects have in common.

\sa Object3D, Object3DAnimated, Object3DAnimatedComplex, Object3DMousable
*/
#include "Object3dAbstract.h"
#include <QDebug>

namespace Cnoti3D {

/*!
Constructs a new state machine with the given parent.
*/
Object3DAbstract::Object3DAbstract( QObject* parent )
	:QObject( parent )
{
}

/*!
Constructs a new state machine with the given parent.
*/
Object3DAbstract::~Object3DAbstract()
{
}

/**************************************************/
/*                    Getters                     */
/**************************************************/
QVector3D Object3DAbstract::position() const
{
	return _position;
}

qreal Object3DAbstract::xPos() const
{
	return _position.x();
}
qreal Object3DAbstract::yPos() const
{
	return _position.y();
}
qreal Object3DAbstract::zPos() const
{
	return _position.z();
}

/**************************************************/
/*                    Setters                     */
/**************************************************/
//void Object3DAbstract::setPosition( qreal x, qreal y, qreal z )
//{
//	this->setPosition( QVector3D( x, y, z) );
//}

void Object3DAbstract::setPosition( const QVector3D& pos )
{
	_position = pos;
}

} // namespace Cnoti3D
