/*!
\class StateManager statemanager.h <StateManager>

The StateManager extends QStateMachine whith information about the active state and last active state.

\sa State, FinalState, QStateMachine
*/

#include "statemanager.h"
namespace Cnoti3D
{
    /*!
    Constructs a new state machine with the given parent.
    */
    StateManager::StateManager(QObject *parent) :
            QStateMachine(parent)
    {
        _activeState = NULL;
        _prevState = NULL;
    }

    /*!
    Activates a state
    */
    void StateManager::setActiveState(QAbstractState* state)
    {
        _prevState = _activeState;
        _activeState = state;
    }

    /*!
    Returns the active state
    */
    QAbstractState* StateManager::activeState() const
    {
        return this->_activeState;
    }

    /*!
    Returns the previous active state
    */
    QAbstractState* StateManager::prevousActiveState() const
    {
        return this->_prevState;
    }
}
