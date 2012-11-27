/*!
\class Cnoti3d Object3dAbstract.h <Cnoti3d>

The Cnoti3d is a abstract base class.
*/
#include "Cnoti3d.h"
#include "Object3dAbstract.h"
#include "ObjectManager.h"
#include "StateManager.h"
// Qt
#include <QGraphicsScene>
#include <QWidget>


namespace Cnoti3D
{

	Cnoti3d::Cnoti3d()
	{
		_3dObjectsMgr = new ObjectManager();
	}

	Cnoti3d::~Cnoti3d()
	{
		delete _3dObjectsMgr;
	}
}
