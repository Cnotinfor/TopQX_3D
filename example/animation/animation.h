#if !defined(_EXAMPLE_H)
#define _EXAMPLE_H

// Cnoti 3D
#include <QtOgre.h>
#include <Character>


class QKeyEvent;
class AnimationSelection;

class AnimationExample : public Cnoti3D::QtOgre
{
	Q_OBJECT
	public:
		AnimationExample(QObject* parent = 0);
		~AnimationExample();

	protected slots:
		void keyPressEvent (QKeyEvent *keyEvent);

	private:
		Cnoti3D::Character* _character;
		AnimationSelection* _animationSelector;

};

#endif  //_EXAMPLE_H
