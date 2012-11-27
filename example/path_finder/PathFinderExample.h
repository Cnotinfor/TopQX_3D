#if !defined(_PATHFINDEREXAMPLE_H)
#define _PATHFINDEREXAMPLE_H

// Cnoti3D
#include <QtOgre>

class QMouseEvent;

class PathFinderExample : public Cnoti3D::QtOgre
{
	public:
		PathFinderExample( QObject* parent = 0 );
		~PathFinderExample();

private:
		Cnoti3D::State *_state;
};

#endif  //_PATHFINDEREXAMPLE_H
