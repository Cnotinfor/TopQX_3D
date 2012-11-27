// 3D Graphics engine
#include "balloon.h"
// Qt
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Cnoti3D::QOgreGraphicsView* view = new Cnoti3D::QOgreGraphicsView();
//    view->resize( 800, 600 );
	Balloon* my3D = new Balloon(/*view*/);
	Q_UNUSED( my3D );
//    view->setAlignment(Qt::AlignCenter);
//    view->show();
    return a.exec();
}
