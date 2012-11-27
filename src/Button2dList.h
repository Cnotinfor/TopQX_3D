#ifndef BUTTON2DLIST_H
#define BUTTON2DLIST_H
/**


 */

//Qt
#include <QList>
#include <QString>

namespace CEGUI {
	class Window;
}

namespace Cnoti3D {

class Button2d;

class Button2dList : public QList<Button2d *>
{
public:
	Button2dList();
	Button2dList(CEGUI::Window *parent);
	~Button2dList();

	Button2d *button(int id);
	Button2d *activeButton(){return _activeButton;}
	void setActiveButton(int id);

	void show();
	void hide();

	void append(Button2d *bt);

private:
	Button2d *_activeButton;
	CEGUI::Window *_ceguiParent;
};

} //namespace Cnoti3D

#endif BUTTON2DLIST_H
