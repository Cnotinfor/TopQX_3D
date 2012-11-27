#ifndef CEGUISTATE_H
#define CEGUISTATE_H

// Cnoti3D
#include <State>

namespace Cnoti3D {
	class Character;
	class Grid;
	class PathFinder;
}

class PathFinderState : public Cnoti3D::State
{
public:
	PathFinderState(Cnoti3D::QtOgre* qtOgre, QState *parent = 0 );
	void onEntry(QEvent *event);
protected:
	void mouseReleasedEvent(QMouseEvent *mouseEvent);
	void keyPressEvent ( QKeyEvent * keyEvent );
	void loadHelpers();
private:
	Cnoti3D::Character* _batutas;
	QList<QVector3D>  _positions;
	Cnoti3D::Grid* _grid;
	Cnoti3D::PathFinder* _pathFinder;
};

#endif // CEGUISTATE_H
