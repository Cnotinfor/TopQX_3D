// 3D Graphics engine
#include "cegui.h"
// Qt
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CeguiExample* my3D = new CeguiExample(/*view*/);
    return a.exec();

	Q_UNUSED( my3D );
}
