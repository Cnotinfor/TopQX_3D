#ifndef OBJECT3DANIMATEDCOMPLEX_H
#define OBJECT3DANIMATEDCOMPLEX_H

#include "Cnoti3d_global.h"
#include "Object3dAnimated.h"
#include "Ogre.h"

#include <QStringList>
#include <QQueue>

namespace Cnoti3D
{
    class CNOTI3DLIB_EXPORT Object3DAnimatedComplex: public Object3DAnimated
    {
    public:
        Object3DAnimatedComplex( Ogre::SceneManager* sceneManager,
                                 const QString& objectName,
                                 const QStringList& animationLst);
        Object3DAnimatedComplex( Ogre::Entity* entity,
                                 const QStringList& animationLst);

        void startAnimation();
        void stopAnimation();

    public slots:
        virtual void update ( int time );

    private:
        bool _loop;
        QStringList _animationLst;
    };
}
#endif // OBJECT3DANIMATEDCOMPLEX_H
