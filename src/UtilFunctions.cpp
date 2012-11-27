#include "UtilFunctions.h"

#include <QSettings>
#include <QDir>

namespace Cnoti3D {

//	UtilFunctions::UtilFunctions()
//	{

//	}

//	UtilFunctions::~UtilFunctions()
//	{

//	}

	Ogre::Vector3 UtilFunctions::qVector3dToOgreVector3( QVector3D vector )
	{
		Ogre::Vector3 ogreVector;
		ogreVector.x = vector.x();
		ogreVector.y = vector.y();
		ogreVector.z = vector.z();
		return ogreVector;
	}

	QVector3D UtilFunctions::ogreVector3ToQVector3d( Ogre::Vector3 vector )
	{
		QVector3D qVector;
		qVector.setX( vector.x );
		qVector.setY( vector.y );
		qVector.setZ( vector.z );
		return qVector;
	}

	qreal UtilFunctions::calculateDistance( QVector3D p1, QVector3D p2 )
	{
		Ogre::Vector3 orig = qVector3dToOgreVector3( p1 );
		Ogre::Vector3 dest = qVector3dToOgreVector3( p2 );
		return orig.distance(dest);
	}

	/*!
		Returns the user system path, where it can store stuff
	*/
	QString UtilFunctions::userSystemPath()
	{
		QSettings s( QSettings::IniFormat, QSettings::UserScope, "Imagina", "App" );
		QString s2 = QDir( s.fileName() ).absolutePath();
		s2.truncate( s2.lastIndexOf( "/" ) + 1 );

		return QDir::toNativeSeparators( s2 );
	}


} //namespace Cnoti3D
