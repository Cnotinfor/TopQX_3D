// 3D Graphics engine
#include "animation.h"
// Qt
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AnimationExample* my3D = new AnimationExample(/*view*/);
	Q_UNUSED( my3D );
    return a.exec();
}
