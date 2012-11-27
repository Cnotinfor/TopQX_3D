#ifndef CNOTI3DSTATEMANAGER_H
#define CNOTI3DSTATEMANAGER_H

#include <QAbstractState>
#include <QObject>
#include <QStateMachine>
#include <QState>

namespace Cnoti3D
{
	class StateManager : public QStateMachine
	{
    public:
        friend class State;
        friend class FinalState;

        StateManager(QObject *parent = 0);
        QAbstractState* activeState() const;
        QAbstractState* prevousActiveState() const;

    private:
        void setActiveState(QAbstractState* state);
        QAbstractState* _activeState;
        QAbstractState* _prevState;

    };
}
#endif // CNOTI3DSTATEMANAGER_H
