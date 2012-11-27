#include "Button2dList.h"
#include "Button2d.h"
// CEGUI
#include <CEGUIWindow.h>
// Qt
#include <QDebug>


namespace Cnoti3D {
/*!
  Constructor for Button
 */
Button2dList::Button2dList() :
	QList<Button2d *>(),
	_ceguiParent(NULL)
{

}

/*!
  Constructor to be used when the buttons are agrouped under a cegui window.
*/
Button2dList::Button2dList(CEGUI::Window *parent) :
	QList<Button2d *>(),
	_ceguiParent(parent)
{

}

Button2dList::~Button2dList()
{

}

void Button2dList::append(Button2d *bt)
{
	QList<Button2d *>::append(bt);
	if(_ceguiParent)
	{
		_ceguiParent->addChildWindow(bt->ceguiButton());
	}
}

Button2d *Button2dList::button(int id)
{
	if(id >= 0 && id < size() )
	{
		return at(id);
	}
	return NULL;
}

/*!
  Activates the button identified by \a id.
*/
void Button2dList::setActiveButton(int id)
{
	for(int i=0; i < size(); i++)
	{
		if(i ==  id)
		{
			at(i)->setSelected(true);
			_activeButton = at(i);
		}
		else
		{
			at(i)->setSelected(false);
		}
	}
}

/*!
  Shows all the buttons in the list.
*/
void Button2dList::show()
{
	if(_ceguiParent)
	{
		_ceguiParent->show();
	}
	else
	{
		for(int i=0; i < size(); i++)
		{
			at(i)->show();
		}
	}
}

/*!
  Hides all the buttons in the list.
*/
void Button2dList::hide()
{
	if(_ceguiParent)
	{
		_ceguiParent->hide();
	}
	else
	{
		for(int i=0; i < size(); i++)
		{
			at(i)->hide();
		}
	}
}

} // namespace Cnoti3D
