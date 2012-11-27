#include "CeguiState.h"
// Cnoti3D
#include <Button2d>
// Qt
#include <QDebug>

CeguiState::CeguiState(Cnoti3D::QtOgre* qtOgre, QState *parent):
	State("CeguiExample", qtOgre, parent)
{
	int _btMain = createButtonList();
	//
	Cnoti3D::Button2d *bt = new Cnoti3D::Button2d("bt_cancel.png", 10, 10);
	connect(bt, SIGNAL(clicked()), this, SLOT(btClicked()));
	addButton(_btMain, bt);

	//
	Cnoti3D::Button2d *bt2 = new Cnoti3D::Button2d("bt_open.png","bt_open_hover.png", "bt_open_hover.png", 10, 140);
	connect(bt2, SIGNAL(clicked()), this, SLOT(btClicked()));
	addButton(_btMain, bt2);

	//
	Cnoti3D::Button2d *bt3 = new Cnoti3D::Button2d("bt_number_1_small.png",
												   "bt_number_1_small_hover.png",
												   "bt_number_1_small_selected.png",
												   "bt_number_1_small_selected_hover.png",
												   140,
												   10);
	connect(bt3, SIGNAL(clicked()), this, SLOT(btClicked()));
	bt->setToolTip(tr("Open a music"));
	addButton(_btMain, bt3);
	bt3->setSelected(true);
}

void CeguiState::btClicked()
{
	qDebug() << "[CeguiState::btClicked]";

}


