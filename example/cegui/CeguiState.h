#ifndef CEGUISTATE_H
#define CEGUISTATE_H

// Cnoti3D
#include <State>

class CeguiState : public Cnoti3D::State
{
	Q_OBJECT
public:
	CeguiState(Cnoti3D::QtOgre* qtOgre, QState *parent = 0 );

public slots:
	void btClicked();
};

#endif // CEGUISTATE_H
