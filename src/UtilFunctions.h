#ifndef CNOTI3D_FUNCTIONS_UTILS_H
#define CNOTI3D_FUNCTIONS_UTILS_H

#include "Cnoti3d_global.h"
// Qt
#include <QVector3D>
// Ogre
#include <Ogre.h>

namespace Cnoti3D {

class CNOTI3DLIB_EXPORT UtilFunctions
{
	public:
//		UtilFunctions();
//		~UtilFunctions();

		static Ogre::Vector3 qVector3dToOgreVector3( QVector3D vector );
		static QVector3D ogreVector3ToQVector3d( Ogre::Vector3 vector );
		static qreal calculateDistance( QVector3D p1, QVector3D p2 );

		static QString userSystemPath();

	private:
		Ogre::RaySceneQuery	*mRayQuery;	/// Scene query ray
};

}	// namespace Cnoti3D


#endif // CNOTI3D_FUNCTIONS_UTILS_H
