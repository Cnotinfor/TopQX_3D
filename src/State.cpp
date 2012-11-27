/*!
\class State state.h <State>

The QState class provides a general-purpose state for StateManager.
State objects can have child states, and can have transitions to other states. State is part of The State Manager wich is based on Qt's State Machine Framework.

Top-level states must be passed a StateManager object as their parent state, or added to a state machine using StateManager::addState().

\sa StateManager, FinalState
*/

#include "state.h"
#include "finalstate.h"
#include "statemanager.h"
#include "QtOgre.h"
#include "Button2dManager.h"
#include "Button2d.h"
#include "OgreWidget.h"
// CEGUI
#include <CEGUIWindowManager.h>
// Qt
#include <QDebug>

namespace Cnoti3D
{
	/*!
	Constructs a new state.
	\param parent   State's parent state
	*/
	State::State(const QString& name, QtOgre* qtOgre, QState *parent) :
		QState(parent),
		_name( name ),
		_qtOgre( qtOgre )
	{
		_button2dMgr = _qtOgre->_buttonsMgr;

		createCeguiWindow();
	}

	/*!
	Constructs a new state.
	\param name     State's name
	\param childMode State's ChildMode
	\param parent   State's parent state
	*/
	State::State(const QString& name, QtOgre* qtOgre, ChildMode childMode, QState *parent) :
		QState( childMode, parent),
		_name( name ),
		_qtOgre( qtOgre )
	{
		if(_qtOgre != NULL)
		{
			_button2dMgr = _qtOgre->_buttonsMgr;
		}

		createCeguiWindow();
	}

	void State::createCeguiWindow()
	{
		// All windows and widgets are created via the WindowManager singleton.
		CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

		// rootWindow
		_ceguiWindow = winMgr.createWindow( "DefaultWindow", _name.toStdString() );
		_ceguiWindow->setProperty("InheritsAlpha", "False");
		_ceguiWindow->setUsingAutoRenderingSurface(false);
		_ceguiWindow->setProperty("UnifiedMaxSize", "{{1,0},{1,0}}");
		_ceguiWindow->setProperty("UnifiedSize", "{{1,0},{1,0}}");
		_ceguiWindow->setProperty("UnifiedAreaRect", "{{0,0},{0,0},{1,0},{1,0}}");
	}

	/*!
	This function is called when the state is entered. The given event is what caused the state to be entered. Reimplement this function to perform custom processing when the state is entered.
	*/
	void State::onEntry(QEvent *event)
	{
		QState::onEntry(event);
		qDebug() << "[State::onEntry] - STATE:" << name();
		// Connect key events
		if(_qtOgre != NULL)
		{
			connect(_qtOgre->ogreWidget(), SIGNAL(keyPressedEvent(QKeyEvent*)), this, SLOT(keyPressEvent(QKeyEvent*)));
			connect(_qtOgre->ogreWidget(), SIGNAL(closing()), this, SIGNAL(exit()));
		}


		resizeGUI( _qtOgre->ogreWidget()->size() );

		CEGUI::System::getSingleton().setGUISheet( _ceguiWindow );
	}

	/*!
	This function is called when the state is exited. The given event is what caused the state to be exited. Reimplement this function to perform custom processing when the state is exited.
	*/
	void State::onExit (QEvent *event)
	{
		// Disconnects all signals from Cnoti3DQtOgre to this class.
		if(_qtOgre != NULL)
		{
			disconnect(_qtOgre->ogreWidget(), 0, this, 0);
			disconnect(_qtOgre->ogreWidget(), SIGNAL(closing()), this, SIGNAL(exit()));
		}

		qDebug() << "[State::onExit] - STATE:" << name();
		QState::onExit(event);
	}

	/*!
	  Sets the Cnoti3DQtOgre pointer.
	  Also is saved the Button2dManager pointer.
	*/
	void State::setCnoti3dQtOgre_Ptr(QtOgre *ptr)
	{
		if(ptr != NULL)
		{
			_qtOgre = ptr;
			_button2dMgr = _qtOgre->_buttonsMgr;
		}
		else
		{
			qWarning() << "[State::setCnoti3dQtOgre_Ptr] Cnoti3dQtOgre object is NULL";
		}
	}

	void State::addButton(int listId, Button2d *bt)
	{
		if(isButtonManagerInitialised())
		{
			// Add to scene
			addCEGUIWindow(bt->ceguiButton());
			// Add to Button Manager
			_button2dMgr->addButton(listId, bt);
		}
	}

	Button2d *State::button(int listId, int btId)
	{
		if(isButtonManagerInitialised())
		{
			return _button2dMgr->button(listId, btId);
		}
		return NULL;
	}

	void State::setActiveButton(int listId, int btId)
	{
		if(isButtonManagerInitialised())
		{
			_button2dMgr->setActiveButton(listId, btId);
		}
	}

	/*!
	  Creates a new button list and returns the list id.
	  If Button Manager is not created asks qtOgre to create and gets the pointer to it.
	  Returns -1 if list wasn´t created.
	*/
	int State::createButtonList()
	{
		qDebug() << "[State::createButtonList] - STATE:" << name();
		// Checks pointers
		if(_qtOgre != NULL && _button2dMgr == NULL)
		{
			_qtOgre->createButtonMananger();
			_button2dMgr = _qtOgre->_buttonsMgr;
		}
		// Checks Button Mananger pointer
		if( _button2dMgr != NULL)
		{
			return _button2dMgr->createButtonList();
		}

		return -1;
	}

	int State::createButtonList(CEGUI::Window *parent)
	{
		qDebug() << "[State::createButtonList] - STATE:" << name();
		// Checks pointers
		if(_qtOgre != NULL && _button2dMgr == NULL)
		{
			_qtOgre->createButtonMananger();
			_button2dMgr = _qtOgre->_buttonsMgr;
		}
		// Checks Button Mananger pointer
		if( _button2dMgr != NULL)
		{
			return _button2dMgr->createButtonList(parent);
		}

		return -1;
	}

	void State::showButtonList(int listId)
	{
		qDebug() << "[State::showButtonList] - STATE:" << name();
		if(isButtonManagerInitialised())
		{
			_button2dMgr->showButtonList(listId);
		}
	}

	void State::showModalButtonList(int listId)
	{
		qDebug() << "[State::showModalButtonList] - STATE:" << name();
		if(isButtonManagerInitialised())
		{
			_button2dMgr->showModalButtonList(listId);
		}
	}

	void State::hideCurrentButtonList(bool showPreviousList)
	{
		qDebug() << "[State::hideCurrentButtonList] - STATE:" << name();
		if(isButtonManagerInitialised())
		{
			_button2dMgr->hideCurrentButtonList(showPreviousList);
		}
	}

	void State::hideButtonList(int listId)
	{
		qDebug() << "[State::hideButtonList] - STATE:" << name();
		if(isButtonManagerInitialised())
		{
			_button2dMgr->hideButtonList(listId);
		}
	}

	void State::hideAllButtons()
	{
		qDebug() << "[State::hideAllButtons] - STATE:" << name();
		if(isButtonManagerInitialised())
		{
			_button2dMgr->hideAll();
		}
	}

	void State::removeButton(int listId, Button2d *bt)
	{
		if(isButtonManagerInitialised())
		{
			// Remove from scene
			_ceguiWindow->removeChildWindow(bt->ceguiButton());
			// Remove from Button Manager
			_button2dMgr->removeButton(listId, bt);
		}
	}

	void State::addCEGUIWindow( CEGUI::Window* window )
	{
        _ceguiWindow->addChildWindow( window );
    }

    void State::removeCEGUIWindow(CEGUI::Window *window)
    {
        _ceguiWindow->removeChildWindow( window );
    }

	bool State::isButtonManagerInitialised()
	{
		if(_button2dMgr == NULL)
		{
			qWarning() << "[State::isButtonManagerInitialised] Button Manager is not initialised";
			return false;
		}
		return true;
	}

	/*!
	This function is called when a keyborad event is raised.
	*/
	void State::keyPressEvent ( QKeyEvent * keyEvent )
	{
		Q_UNUSED(keyEvent);
	}

	void State::mouseReleasedEvent(QMouseEvent *mouseEvent)
	{
		Q_UNUSED(mouseEvent);
	}

	/*
		Resize the GUI
	*/
	void State::resizeGUI(const QSize& size)
	{

//		if(_ceguiWindow != NULL)
//		{
////			CEGUI::UVector2 vector = CEGUI::UVector2(CEGUI::UDim(0, size.width()),
////												   CEGUI::UDim(0, size.height()));
//			CEGUI::UVector2 vector = CEGUI::UVector2(CEGUI::UDim(1, 0),
//												   CEGUI::UDim(1, 0));
//			CEGUI::UVector2 origin = CEGUI::UVector2(CEGUI::UDim(0, 0),
//												   CEGUI::UDim(0, 0));
//			_ceguiWindow->setSize(vector );
//			_ceguiWindow->setArea(origin, vector);
//			_ceguiWindow->setMaxSize(vector);
//			_ceguiWindow->setProperty("UnifiedMaxSize","{{1,0},{1,0}}" );
//			_ceguiWindow->setProperty("UnifiedAreaRect","{{0,0},{0,0},{1,0},{1,0}}" );
//		}
	}

	/*!
	Returns the state name prepended with parent's name.
	Mainly used for debugging
	*/
	QString State::name()const
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
	Returns the tree depth of this state. Root states depth is 0.
	*/
	int State::depth()const
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

