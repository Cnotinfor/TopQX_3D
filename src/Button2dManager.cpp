#include "Button2dManager.h"
#include "Button2dList.h"
#include "Button2d.h"
// Qt
#include <QDebug>


namespace Cnoti3D {

/*!
  Constructor for Button
 */
Button2dManager::Button2dManager(QObject *parent) :
	QObject(parent),
	_modal(false),
	_currentButtonList(INVALID_LIST),
	_lastButtonList(INVALID_LIST)
{

}

Button2dManager::~Button2dManager()
{

}

/*!
  Adds a button to the list with the id \a listId.
*/
void Button2dManager::addButton(int listId, Button2d *bt)
{
	qDebug() << "[Button2dManager::addButton] ListID:" << listId;
	if(isListValid(listId))
	{
		_list[listId]->append(bt);
	}
}

/*!
  Adds a button to the list with the id \a listId.
  Is created a button with the images \a image_normal, \a image_hove, \a image_down and in the position \a x, \a y.
*/
void Button2dManager::addButton(int listId, const QString &image_normal, const QString &image_hover, const QString &image_down, qreal x, qreal y)
{
	qDebug() << "[Button2dManager::addButton(int, QString, ...)] ListID:" << listId;
	if(isListValid(listId))
	{
		addButton(listId, new Button2d(image_normal, image_hover, image_down, x, y, this));
	}
}

/*!
  Gets the active button for the list with the id \a listId.
*/
Button2d *Button2dManager::activeButton(int listId)
{
	qDebug() << "[Button2dManager::activeButton] ListID:" << listId;
	if(isListValid(listId))
	{
		return _list[listId]->activeButton();
	}
	return NULL;
}

/*!
  Sets the active button for the list with the id \a listId.
*/
void Button2dManager::setActiveButton(int listId, int btId)
{
	qDebug() << "[Button2dManager::setActiveButton] ListID:" << listId << "ButtonID:" << btId;
	if(isListValid(listId))
	{
		_list[listId]->setActiveButton(btId);
	}
}

/*!
  Removes a button to the list with the id \a listId.
*/
void Button2dManager::removeButton(int listId, Button2d *bt)
{
	qDebug() << "[Button2dManager::addButton] ListID:" << listId;
	if(isListValid(listId))
	{
		if(_list[listId]->contains(bt))
		{
			_list[listId]->removeOne(bt);
		}
	}
}

/*!
  Creates an empty list.
  Returns the id of the new list.
*/
int Button2dManager::createButtonList()
{
	qDebug() << "[Button2dManager::createButtonList]";
	return addButtonList(new Button2dList());
}

/*!
  Creates an empty list.
  Returns the id of the new list.
*/
int Button2dManager::createButtonList(CEGUI::Window *parent)
{
	qDebug() << "[Button2dManager::createButtonList]";
	return addButtonList(new Button2dList(parent));
}

/*!
  Adds the button list to the managed itens.
  Return the id of the added list.
*/
int Button2dManager::addButtonList(Button2dList *list)
{
	qDebug() << "[Button2dManager::addButtonList]";
	int listID = _list.size();
	_list << list;
	return listID;
}

/*!
  Gets the button identified by \a id in the list \a listID.
*/
Button2d *Button2dManager::button(int listId, int btId)
{
	qDebug() << "[Button2dManager::buttonList] ListID:" << listId << "ButtonID:" << btId;
	if(isListValid(listId))
	{
		return _list[listId]->button(btId);
	}
	return NULL;
}

/*!
  Gets the the list with the id \a listId.
*/
Button2dList *Button2dManager::buttonList(int listId)
{
	qDebug() << "[Button2dManager::buttonList] ListID:" << listId;
	if(isListValid(listId))
	{
		return _list[listId];
	}
	return NULL;
}

void Button2dManager::showButtonList(int listId)
{
	qDebug() << "[Button2dManager::showButtonList] ListID:" << listId;
	// Hides current button list
	if(_currentButtonList != INVALID_LIST)
	{
		hideButtonList(_currentButtonList);
	}
	// Show button list
	if(isListValid(listId))
	{
		_list[listId]->show();
		_currentButtonList = listId;
		qDebug() << "[Button2dManager::showButtonList] Showing ListID:" << listId;
	}
}

void Button2dManager::showModalButtonList(int listId)
{
	qDebug() << "[Button2dManager::showModalButtonList] ListID:" << listId;
	if(isListValid(listId))
	{
		showButtonList(listId);
		setModalState(true);
	}
}

/*!
  Hides a button list identified by it's id.
*/
void Button2dManager::hideButtonList(int listId)
{
	qDebug() << "[Button2dManager::hideButtonList] ListID:" << listId;
	if(isListValid(listId))
	{
		if(isShowingModalButtons())
		{
			setModalState(false);
		}
		_list[listId]->hide();
		if(_currentButtonList == listId)
		{
			// Saves the current list
			_lastButtonList = _currentButtonList;
			_currentButtonList = INVALID_LIST;
		}
	}
}

/*!
  Hides the visible buttons and shows previous visible buttons when \a showPreviousList is true.
*/
void Button2dManager::hideCurrentButtonList(bool showPreviousList)
{
	qDebug() << "[Button2dManager::hideCurrentButtonList]";
	int previousID = _lastButtonList;	// in case of need showing the last list (is changes when list is hided).

	// Hide
	hideButtonList(_currentButtonList);

	if(showPreviousList)
	{
		// Show previous list
		showButtonList(previousID);
	}
}

/*!
  Hides all buttons.
*/
void Button2dManager::hideAll()
{
//	QListIterator<Button2dList *> it(_list);
//	while(it.hasNext())
//	{
//		it.next()->hide();
//	}
	for(int i=0; i < _list.size(); i++)
	{
		hideButtonList(i);
	}
}

bool Button2dManager::isListValid(int listId)
{
	if(listId < 0 || listId >= _list.size())
	{
		qWarning() << "[Button2dManager::isListValid] ID list is not valid. ListID:" << listId;
		return false;
	}
	if(_list[listId] == NULL)
	{
		qWarning() << "[Button2dManager::isListValid] List is NULL";
		return false;
	}

	return true;
}

/*!
  Gets the id of the Button2dList \a list.
  Returns -1 (INVALID_LIST) if list not found.
*/
int Button2dManager::buttonListId(Button2dList *list)
{
	for(int i=0; i < _list.size(); i++)
	{
		if(_list[i] == list)
		{
			return i;
		}
	}
	return INVALID_LIST;
}

} // namespace Cnoti3D
