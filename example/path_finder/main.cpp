// 3D Graphics engine
#include "PathFinderExample.h"
// Qt
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PathFinderExample* app = new PathFinderExample();
    return a.exec();

	Q_UNUSED( app );
}
