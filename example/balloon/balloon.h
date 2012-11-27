#if !defined(_EXAMPLE_H)
#define _EXAMPLE_H

#include <QtOgre>
#include <Object3DAnimated>
#include <Object3DAnimatedComplex>
#include <QOgreGraphicsView>
#include <Blueprint>
#include <QPointer>

class QMouseEvent;

class Balloon : public Cnoti3D::Cnoti3dQtOgre
{
	public:
		Balloon( QObject* parent = 0 );
		~Balloon();

//        void keyPressEvent ( QKeyEvent * keyEvent );
//        void keyReleaseEvent ( QKeyEvent * keyEvent );
//        Cnoti3D::Blueprint* _map;

//        virtual void buildScene( Cnoti2D::LoadManager *lm );
//        virtual void toLoad( Cnoti2D::LoadManager *lm );

	private:
		//Cnoti3dQtOgre* _cnoti3D;
		//Cnoti3D::Object3DAnimatedComplex* _obj;
		//Cnoti2D::SceneManager* _sceneManager;

};

#endif  //_EXAMPLE_H
