/*!
\class FinalState state.h <FinalState>

The FinalState class provides a general-purpose final state for StateManager.
A final state is used to communicate that (part of) a StateManager has finished its work.
When a final top-level state is entered, the state machine's finished() signal is emitted.
In general, when a final substate (a child of a State) is entered, the parent state's finished() signal is emitted. FinalState is part of State Manager.

\sa StateManager, State
*/

#include "finalstate.h"

#include "statemanager.h"
#include "state.h"

#include <QDebug>

namespace Cnoti3D
{
    /*!
    Constructs a new final state.
    \param name     State's name
    \param sm       State's SceneManager
    \param parent   State's parent state
    */
    FinalState::FinalState(const QString& name,
                           StateManager* sm,
                           QState *parent)
        :QFinalState(parent)
    {
        _stateManager = sm;
        _name = name;
    }

    /*!
    Constructs a new final state.
    \param name     State's name
    \param sm       State's SceneManager
    */
    FinalState::FinalState(const QString& name,
               QState *parent)
            :QFinalState(parent)
    {
        _stateManager = NULL;
        _name = name;
    }

    /*!
    This function is called when the state is entered. The given event is what caused the state to be entered. Reimplement this function to perform custom processing when the state is entered.
    */
    void FinalState::onEntry(QEvent *event)
    {
        Q_UNUSED(event)
        if(_stateManager)
        {
            _stateManager->setActiveState(this);
        }
    }

    /*!
    Returns the state name prepended with parent's name.
    Mainly used for debugging.
    */
    QString FinalState::name()const
    {
        State* stt;
        FinalState* fstt;

        if(parent()!=NULL)
        {
            stt = dynamic_cast<State*> (parent());
            if(stt!=NULL)
            {
                return stt->name().append(":").append(_name);
            }else
            {
                fstt = dynamic_cast<FinalState*> (parent());
                if(fstt!=NULL)
                {
                    return fstt->name().append(":").append(_name);
                }else
                {
                    return QString("NA:").append(_name);
                }
            }
        }
        return _name;
    }

    /*!
    Returns the tree depth of this state. Root state's depth is 0.
    */
    int FinalState::depth()const
    {
        State* stt;
        FinalState* fstt;

        if(parent()!=NULL)
        {
            stt = dynamic_cast<State*> (parent());
            if(stt!=NULL)
            {
                return stt->depth()+1;
            }else
            {
                fstt = dynamic_cast<FinalState*> (parent());
                if(fstt!=NULL)
                {
                    return fstt->depth()+1;
                }else
                {
                    qWarning()<<"[State::depth]: Depth cannot be determined.";
                }
            }
        }
        return 0;
    }
}
