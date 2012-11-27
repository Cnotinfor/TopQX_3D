/*!
\class BandMember bandmember.h
This class represents a Band Member
This is a container class that contains Body and can also contain the state machine for its control.

\sa BandMemberBody
*/
#include "CharacterMovement.h"
#include "Object3dAbstract.h"
#include "UtilFunctions.h"
// Qt
#include <QDebug>

namespace Cnoti3D {

CharacterMovement::CharacterMovement( Ogre::Node* node ):
	QObject(),
	_node (node),
	_moving (false),
	_speedMultiplier (1),
	_minDistance(20)
{
	// Set the character speed
	_speed = 95.0 * _speedMultiplier;
//	_rotationInc = 1.0f / 15.0f;	// takes us 15 frames to make a turn
	_rotationInc = 1.0f / 5.0;	// takes us 5 frames to make a turn
}

CharacterMovement::~CharacterMovement()
{

}

void CharacterMovement::addPointToMove( QVector3D destination )
{
	qDebug() << "[CharacterMovement::addPointToMove] - Destination:" << destination ;
	_moveList.push_back( destination );
}

void CharacterMovement::moveTo(QVector3D destination)
{
	// new destination
	_destination = destination;
	stopMoving();
	//
	qreal dist = UtilFunctions::calculateDistance(_position, destination);
	if (dist <= _minDistance)
	{
		qWarning() << "[CharacterMovement::moveTo(QVector3D)] Distance to next point is smaller than the minimum distance variable." << dist;
		qDebug() << "[CharacterMovement::moveTo(QVector3D)] - Emited stopedMoving";
		emit stopedMoving();
	}
	else
	{
		moveTo();
	}
}


void CharacterMovement::moveTo( QVector3D destination, QString destinationName, bool forced )
{
	// new destination name
	_destinationName = destinationName;
	//
	_moveList.clear();
	moveTo(destination);
}

void CharacterMovement::moveTo(QList<QVector3D> destinationList)
{
	if(!destinationList.isEmpty())
	{
		// save list of point
		_moveList = destinationList;
		// move to first point and removes it from the list
		moveTo(_moveList.takeFirst());
	}
}

void CharacterMovement::moveTo()
{
	qDebug() << "[CharacterMovement::moveTo]";
	// Get current position
	_position = UtilFunctions::ogreVector3ToQVector3d( _node->getPosition() );

	// clear the list of point
//	_moveList.clear();

	// Compute distance to new location
	_distance = UtilFunctions::calculateDistance( _position, _destination );
	qDebug() << "[CharacterMovement::moveTo] - Distance:" << _distance << "Pos:" << _position << "Dest:" << _destination;
	if( _distance > 0.000001f )
	{
		// compute direction vector
		_direction = _destination - _position;
		_direction.normalize();
		_moving = true;
		rotateTo(_destination);
		qDebug() << "[CharacterMovement::moveTo] - Emited startedMoving";
		emit startedMoving();
	}
}

void CharacterMovement::rotateTo( QVector3D directionLook )
{
	qDebug() << "[CharacterMovement::rotateTo]" << directionLook;
	Ogre::Vector3 newDirection = UtilFunctions::qVector3dToOgreVector3( directionLook ) - _node->getPosition();
	newDirection.y = 0;
	newDirection.normalise();
	// Check if it is necessary to rotate
//	Ogre::Vector3 auxDirection = UtilFunctions::qVector3dToOgreVector3( _direction );

//	auxDirection.normalise();

//	qDebug() << "[CharacterMovement::rotateTo]" << directionLook << newDirection.dotProduct( auxDirection );

//	if( newDirection.dotProduct( auxDirection ) != 1 )
	{
		_direction = UtilFunctions::ogreVector3ToQVector3d( newDirection );
		_rotationValue = -4.0f;	// TODO: find another was to stop the rotation (it wasn't stopping)
		_rotating = true;
	}
}


void CharacterMovement::rotate()
{
	if( !_rotating )
	{
		return;
	}

	// linear interpolate
	_rotationValue += _rotationInc;
	if( _rotationValue > 1.0 )
	{
		_rotating = false;
	}
	else
	{
		// Update player's orientation
		Ogre::Vector3 facingVector = _node->getOrientation() * Ogre::Vector3::UNIT_Z;
		Ogre::Vector3 direction = UtilFunctions::qVector3dToOgreVector3( _direction );

		// Work around 180 degree quaternion rotation quirk
		// avoid divide by zero
		// this works our only in case we are walking on non-regular terrain..
		// its irrelevant on plane ground
		if( (1.0f + facingVector.dotProduct( direction )) < 0.0001f )
		{
			_node->yaw( Ogre::Degree(180) );
		}
		else
		{
			Ogre::Quaternion delta;
			Ogre::Quaternion orientationSrc = _node->getOrientation();
			Ogre::Quaternion orientationDest = facingVector.getRotationTo( direction ) * orientationSrc;
			// Slerp
			delta = Ogre::Quaternion::Slerp( _rotationInc, orientationSrc, orientationDest );

			_node->setOrientation( delta );
		}
	}
}

void CharacterMovement::move( float time )
{
	if( !_moving )
	{
		return;
	}

	rotateTo(_destination);
	QVector3D position = _position;
	if( _distance <= 0 /*&& transitionToStop == false*/ )
	{
		//if this node isn't the last
		if( !_moveList.empty() )
		{
			_destination = _moveList[0];
			_distance = UtilFunctions::calculateDistance( position, _destination );
			if( _distance > 0.000001f )
			{
				// compute direction vector
				rotateTo(_destination);
			}

			// delete the first node on the list
			_moveList.erase( _moveList.begin() );
		}
		else
		{
			// the player arrive to last destination
			this->stopMoving();
		}
	}

	QVector3D dir = (_direction * _speed) * time;
	if( _distance > 0.000001f )//&& animationState->getAnimationName() != "Bow" )
	{
		Ogre::Real newDistance = dir.length();
		// Update player's position
		// interpolate linearly
		if( newDistance > _distance )
		{
			position = _destination;
			_distance = 0;
		}
		else
		{
			_distance -= newDistance;
			position += dir;
		}
		_position = position;
		_node->setPosition( UtilFunctions::qVector3dToOgreVector3( _position ) );
	}
	else
	{
//		if( transitionToStop )
		{
			_position = position;
			_node->setPosition( UtilFunctions::qVector3dToOgreVector3( _position ) );
		}
	}
}

/*!
	Stops moving character.
*/
void CharacterMovement::stopMoving()
{
	if(_moving)
	{
		qDebug() << "[CharacterMovement::stopMoving] - Emited stopedMoving";
		_moving = false;
		_moveList.clear();
		emit stopedMoving();
	}
	// Look
	this->rotateTo(_lookTo);
}

void CharacterMovement::update( float time )
{
	this->move( time );
	this->rotate();
}

/***************************************/
/*             GETTERS                 */
/***************************************/
QVector3D CharacterMovement::getLastPointMoveList()
{
	int last = _moveList.size();
	if( last >0 )
	{
		return _moveList[last-1];
	}
	else
	{
		return QVector3D(9999,9999,9999);
	}
}

/***************************************/
/*             SETTERS                 */
/***************************************/

void CharacterMovement::setSpeedMultiplier( float speed )
{
	if( speed >= 0)
	{
		_speedMultiplier = speed; // Set the character speed
		_speed = 95.0 * _speedMultiplier;
	}
	else
	{
		qWarning() << "[CharacterMovement::setSpeedMultiplier] Invalid speed value:" << speed;
	}
}

/*!
	Changes the moving status (true or false).
	Only changed to true if character starts moving to a new destination.
*/
void CharacterMovement::setMoving( bool startMoving )
{
	if( startMoving )
	{
		moveTo();
	}
	else
	{
		stopMoving();
	}
}

} // namespace Cnoti3D
