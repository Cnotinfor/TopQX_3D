#if !defined(_EXAMPLE_H)
#define _EXAMPLE_H

// Cegui
#include <CEGUIEventArgs.h>
// Cnoti3D
#include <QtOgre>

class QMouseEvent;

class CeguiExample : public Cnoti3D::QtOgre
{
	public:
		CeguiExample( QObject* parent = 0 );
		~CeguiExample();

		bool onLeftClick(const CEGUI::EventArgs &pEventArgs);

private:
		Cnoti3D::State *_state;

		void usingCEGUI();
		void usingCnoti3D();
		void resources();
};

#endif  //_EXAMPLE_H
