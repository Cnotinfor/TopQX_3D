#include "AnimationSelection.h"
#include "ui_AnimationSelection.h"
// Cnoti 3D
#include <Character>

AnimationSelection::AnimationSelection(Cnoti3D::Character* character, QStringList animationList, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AnimationSelection),
	_character(character)
{
    ui->setupUi(this);

	animationList.sort();
	QStringListIterator it(animationList);
	while(it.hasNext())
	{
		ui->comboBox->addItem(it.next());
	}

	connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboboxChanged(QString)));
	connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(playAnimationPressed()));
	connect(ui->pushButton_2, SIGNAL(pressed()), this, SLOT(playNextAnimationPressed()));
}

AnimationSelection::~AnimationSelection()
{
    delete ui;
}

void AnimationSelection::playAnimationPressed()
{
	_character->startAnimation(ui->lineEdit->text(), loop());
}

void AnimationSelection::playNextAnimationPressed()
{
	int currentIndex = ui->comboBox->currentIndex();
	if (currentIndex + 1 >= ui->comboBox->count())
	{
		currentIndex = -1;
	}
	ui->comboBox->setCurrentIndex( currentIndex + 1);

	_character->startAnimation(ui->lineEdit->text(), loop());
}

void AnimationSelection::comboboxChanged(const QString &text)
{
	ui->lineEdit->setText(text);
}

bool AnimationSelection::loop()
{
	return ui->checkBox->isChecked();
}
