#ifndef CNOTI3DSTATE_H
#define CNOTI3DSTATE_H

#include "Cnoti3d_global.h"
// CEGUI
#include <CEGUIWindow.h>
// Qt
#include <QObject>
#include <QState>
#include <QString>
#include <QKeyEvent>

namespace Cnoti3D
{
	class QtOgre;
	class Button2d;
	class Button2dManager;

	class CNOTI3DLIB_EXPORT State : public QState
	{
		Q_OBJECT
	public:
		friend class QtOgre;

		State(const QString& name, QtOgre *qtOgre, QState *parent = 0 );
		State(const QString& name, QtOgre *qtOgre, ChildMode childMode, QState *parent = 0 );

		QString name()const;
		int depth()const;

	public slots:
		virtual void keyPressEvent ( QKeyEvent * keyEvent );
		virtual void mouseReleasedEvent ( QMouseEvent * mouseEvent );
		void resizeGUI(const QSize& size);

	signals:
		void exit();

	protected:
		QtOgre *_qtOgre;
		// Methods
		virtual void onEntry(QEvent *event);
		virtual void onExit(QEvent *event);
		//	Buttons
		void addButton(int listId, Button2d *bt);
		Button2d *button(int listId, int btId);
		void setActiveButton(int listId, int btId);
		int createButtonList();
		int createButtonList(CEGUI::Window *parent);
		void showButtonList(int listId);
		void showModalButtonList(int listId);
		void hideCurrentButtonList(bool showPreviousList = false);
		void hideButtonList(int listId);
		void hideAllButtons();
		void removeButton(int listId, Button2d *bt);

		void addCEGUIWindow( CEGUI::Window* window );
        void removeCEGUIWindow( CEGUI::Window* window );

	private:
		QString _name;
		Button2dManager *_button2dMgr;
		// Methods
		void setCnoti3dQtOgre_Ptr(QtOgre *ptr);
		bool isButtonManagerInitialised();

	// CEGUI
	private:
		CEGUI::Window* _ceguiWindow;

		// Methods
		void createCeguiWindow();
	};
}
#endif // CNOTI3DSTATE_H
