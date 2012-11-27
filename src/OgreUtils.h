#ifndef OGREUTILS_H
#define OGREUTILS_H

#include "Cnoti3d_global.h"

#include <Ogre.h>

namespace Cnoti3D
{
	class OgreUtils
	{
		public:
			OgreUtils();
			OgreUtils( Ogre::SceneManager* sceneMgr );
			~OgreUtils();

			static Ogre::Root* ogreRoot_Ptr();
			static void getMeshInformation( const Ogre::MeshPtr mesh, size_t &vertex_count, Ogre::Vector3* &vertices, size_t &index_count, unsigned long* &indices, const Ogre::Vector3 &position, const Ogre::Quaternion &orient, const Ogre::Vector3 &scale );

			bool rayCastFromPoint( Ogre::Vector3 &_p, Ogre::Vector3 &_n, Ogre::Vector3 &_result );
			bool rayCastFromPoint( Ogre::Ray &_ray, Ogre::Vector3 &_result );
			bool rayCastFromPoint( Ogre::Ray &_ray, Ogre::Vector3 &_result, Ogre::String &_name );

		private:
			Ogre::RaySceneQuery	*mRayQuery;	/// Scene query ray
	};
}


#endif // OGREUTILS_H
