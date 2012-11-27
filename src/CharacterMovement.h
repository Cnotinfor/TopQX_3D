#ifndef CHARACTERMOVEMENT_H
#define CHARACTERMOVEMENT_H

#include "Cnoti3d_global.h"
// Qt
#include <QObject>
#include <QVector3D>
// Ogre
#include <Ogre.h>


namespace Cnoti3D {

class CNOTI3DLIB_EXPORT CharacterMovement: public QObject
{
	Q_OBJECT
	Q_PROPERTY( QVector3D destination READ destination WRITE setDestination )
	Q_PROPERTY( QString destinationName READ destinationName WRITE setDestinationName )
	Q_PROPERTY( bool moving READ isMoving WRITE setMoving )
public:

	CharacterMovement( Ogre::Node* node );
	~CharacterMovement();

	void addPointToMove( QVector3D destination );
	void moveTo(QVector3D destination);
	void moveTo( QVector3D destination, QString destinationName, bool forced=true );
	void moveTo(QList<QVector3D> destinationList);
	void moveTo();
	void rotateTo( QVector3D directionLook );

	void rotate();
	void move( float time );
	void update( float time );

	void stopMoving();

//	// Getters
	QVector3D getLastPointMoveList();
	QVector3D destination(){ return _destination; }
	bool isMoving(){ return _moving; }
	bool isRotating(){ return _rotating; }
	QString destinationName(){ return _destinationName; }

//	// Setters
	void setSpeedMultiplier( float speed );
	void setDestination( const QVector3D &dest ){ _destination = dest; }
	void setDestinationName( const QString &destName ){ _destinationName = destName; }
	void setMoving( bool start );
	void setLookTo(QVector3D point){_lookTo = point;}

signals:
	void startedMoving();
	void stopedMoving();

private:
	Ogre::Node* _node;
	QVector3D _lookTo;	// Whre to look when stops walking

	float _speed;
	float _speedMultiplier;
	float _rotationValue;
	float _rotationInc;
	float _distance;
	float _minDistance; // Value with the minimum distance between steps


	bool _moving;
	bool _rotating;

	QVector3D _position;
	QString _destinationName;
	QVector3D _destination;
	QVector3D _direction;

	QList<QVector3D> _moveList;		// list of points to move to get to destination

	// FUNCIONS
};

} // namespace Cnoti3D
#endif // CHARACTERMOVEMENT_H
