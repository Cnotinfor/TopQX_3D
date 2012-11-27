#ifndef QTCEGUI_H
#define QTCEGUI_H

#include "../Cnoti3d_global.h"
//CEGUI
#include <CEGUIInputEvent.h>
#include <CEGUISystem.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
//Ogre
#include <Ogre.h>
//Qt
#include <QApplication>

namespace Cnoti3D
{
	class CNOTI3DLIB_EXPORT QtCegui
	{
	public:
		QtCegui();

		unsigned int convertQtToCEGUIKey(int key, Qt::KeyboardModifiers modifiers);
		CEGUI::MouseButton convertQtToCEGUIMouseButton(Qt::MouseButton button);

		bool initialiseGui(Ogre::RenderWindow *renderWindow = NULL);
		void updateGui(qreal timeSinceLastFrame);

		void loadGuiScene(QString fileName);

		static void setCeguiCursor(const QString &imageset, const QString &image_name);

		const QSize ceguiImageSize(const QString &image, const QString &imageset);
		const QSize ceguiImageSize(const QString &image);

        static CEGUI::OgreRenderer* ceguiRenderer();

	private:
		CEGUI::Window* myRoot;
        static CEGUI::OgreRenderer* _renderer;
	};
} // namespace Cnoti3D

#endif QTCEGUI_H
