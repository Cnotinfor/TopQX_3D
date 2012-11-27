#ifndef ANIMATIONSELECTION_H
#define ANIMATIONSELECTION_H

#include <QWidget>

namespace Ui {
    class AnimationSelection;
}

namespace Cnoti3D {
	class Character;
}

class AnimationSelection : public QWidget
{
    Q_OBJECT

public:
	explicit AnimationSelection(Cnoti3D::Character* character, QStringList animationList, QWidget *parent = 0);
    ~AnimationSelection();

private:
    Ui::AnimationSelection *ui;
	Cnoti3D::Character* _character;

	bool loop();

private slots:
	void playAnimationPressed();
	void playNextAnimationPressed();
	void comboboxChanged(const QString &text);
};

#endif // ANIMATIONSELECTION_H
