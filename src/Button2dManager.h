#ifndef BUTTON2DMANAGER_H
#define BUTTON2DMANAGER_H
/**


 */

//Qt
#include <QObject>
#include <QString>

namespace CEGUI {
	class Window;
}

namespace Cnoti3D {

class Button2d;
class Button2dList;

class Button2dManager : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool modal READ isShowingModalButtons WRITE setModalState)

public:
	Button2dManager(QObject *parent = 0);
	~Button2dManager();

	void addButton(int listId, Button2d *bt);
	void addButton(int listId, const QString &image_normal, const QString &image_hover, const QString &image_down, qreal x, qreal y);
	Button2d *button(int listId, int btId);
	Button2d *activeButton(int listId);
	void setActiveButton(int listId, int btId);
	void removeButton(int listId, Button2d *bt);

	int createButtonList();
	int createButtonList(CEGUI::Window *parent);
	int addButtonList(Button2dList *list);
	Button2dList *buttonList(int listId);

	void showButtonList(int listId);
	void showModalButtonList(int listId);
	void hideCurrentButtonList(bool showPreviousList = false);
	void hideButtonList(int listId);
	void hideAll();

	bool isShowingModalButtons(){return _modal;}
	void setModalState(bool value){_modal = value;}


private:
	enum EnumList{ INVALID_LIST = -1};

	bool _modal;
	QList<Button2dList *> _list;
	int _lastButtonList;
	int _currentButtonList;

	// Functions
	bool isListValid(int listId);
	int buttonListId(Button2dList *list);
};

} // namespace Cnoti3D

#endif BUTTON2DMANAGER_H
