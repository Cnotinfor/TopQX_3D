#include "qtCegui.h"
//CEGUI
#include <CEGUISchemeManager.h>
#include <CEGUIWindowManager.h>
#include <CEGUIDefaultLogger.h>
#include <CEGUIImageset.h>
#include <CEGUIImagesetManager.h>
// Qt
#include <QDebug>
//
#include "interface/qtCeguiLogger.h"

namespace Cnoti3D
{
    CEGUI::OgreRenderer* QtCegui::_renderer = NULL;

	QtCegui::QtCegui() :
        myRoot(NULL)
	{

	}

	bool QtCegui::initialiseGui(Ogre::RenderWindow *renderWindow)
	{
		new qtCeguiLogger();
		//CEGUI::Logger::getSingleton().setLogFilename("./cegui.log");

		if(renderWindow == NULL)
		{
			 _renderer = &CEGUI::OgreRenderer::bootstrapSystem();
		}
		else
		{
			 _renderer = &CEGUI::OgreRenderer::bootstrapSystem((*renderWindow));
		}

		// Load the scheme
		CEGUI::SchemeManager::getSingleton().create( "AquaLookSkin.scheme" );

		// Set the default Font
        CEGUI::System::getSingleton().setDefaultFont( "DejaVuSans-13" );

		// Set the default Mouse
		CEGUI::System::getSingleton().setDefaultMouseCursor( "AquaLook", "MouseEmpty" );

		// Create default ToolTip item
		CEGUI::System::getSingleton().setDefaultTooltip( (CEGUI::utf8*)"AquaLook/Tooltip" );

		return true;
	}

	void QtCegui::updateGui(qreal timeSinceLastFrame)
	{
		if (CEGUI::System::getSingletonPtr())
		{
			CEGUI::System::getSingleton().injectTimePulse( timeSinceLastFrame );
		}
	}

	void QtCegui::loadGuiScene(QString fileName)
	{
		// Load the scheme
		CEGUI::SchemeManager::getSingleton().create( fileName.toStdString() );
	}

	void  QtCegui::setCeguiCursor(const QString &imageset, const QString &image_name)
	{
		CEGUI::MouseCursor::getSingleton().setImage(imageset.toStdString(), image_name.toStdString());
	}

	const QSize QtCegui::ceguiImageSize(const QString &image, const QString &imageset)
	{
		QSize size;
		CEGUI::ImagesetManager::ImagesetIterator it(CEGUI::ImagesetManager::getSingleton().getIterator());
		it.toStart();
		while(!it.isAtEnd())
		{
			QString name(it.getCurrentValue()->getName().c_str());
			if(name == imageset)
			{
				CEGUI::Imageset* imageSet = it.getCurrentValue();
				CEGUI::Size s;
				try{
					s = imageSet->getImageSize(image.toStdString());
				}
				catch (CEGUI::UnknownObjectException)
				{
					qWarning() << "[QtCegui::ceguiImageSize]" << image << "not found in imageset" << imageSet;
					return size; // return empty size
				}
				size.setWidth(s.d_width);
				size.setHeight(s.d_height);
				return size;
			}
			it++;
		}

		return size; // return empty size
	}

	const QSize QtCegui::ceguiImageSize(const QString &image)
	{
		QSize size;
		CEGUI::ImagesetManager::ImagesetIterator it(CEGUI::ImagesetManager::getSingleton().getIterator());
		it.toStart();
		while(!it.isAtEnd())
		{
			CEGUI::Imageset* imageSet = it.getCurrentValue();
			it++;
			CEGUI::Size s;
			try
			{
				s = imageSet->getImageSize(image.toStdString());
			}
			catch (CEGUI::UnknownObjectException)
			{
				continue;
			}
			size.setWidth(s.d_width);
			size.setHeight(s.d_height);
			return size;
		}

		qWarning() << "[QtCegui::ceguiImageSize]" << image << "not found";
        return size; // return empty size
    }

    CEGUI::OgreRenderer *QtCegui::ceguiRenderer()
    {
        return _renderer;
    }

	unsigned int QtCegui::convertQtToCEGUIKey (int key, Qt::KeyboardModifiers modifiers)
	{
		if (modifiers & Qt::KeypadModifier)
		{
			switch (key)
			{
				case Qt::Key_Minus:	return CEGUI::Key::Subtract;
				case Qt::Key_Period:	return CEGUI::Key::Decimal;
				case Qt::Key_Slash:	return CEGUI::Key::Divide;
				case Qt::Key_Enter:	return CEGUI::Key::NumpadEnter;
				case Qt::Key_0:		return CEGUI::Key::Numpad0;
				case Qt::Key_1:		return CEGUI::Key::Numpad1;
				case Qt::Key_2:		return CEGUI::Key::Numpad2;
				case Qt::Key_3:		return CEGUI::Key::Numpad3;
				case Qt::Key_4:		return CEGUI::Key::Numpad4;
				case Qt::Key_5:		return CEGUI::Key::Numpad5;
				case Qt::Key_6:		return CEGUI::Key::Numpad6;
				case Qt::Key_7:		return CEGUI::Key::Numpad7;
				case Qt::Key_8:		return CEGUI::Key::Numpad8;
				case Qt::Key_9:		return CEGUI::Key::Numpad9;
				case Qt::Key_Asterisk:	return CEGUI::Key::Multiply;
				case Qt::Key_Plus:	return CEGUI::Key::Add;
				case Qt::Key_Equal:	return CEGUI::Key::NumpadEquals;
			}
		}
		else
		{
			switch (key)
			{
				case Qt::Key_At:		return CEGUI::Key::At;
				case Qt::Key_Backspace: 	return CEGUI::Key::Backspace;
				case Qt::Key_Tab:		return CEGUI::Key::Tab;
				case Qt::Key_Pause:		return CEGUI::Key::Pause;
				case Qt::Key_Escape:		return CEGUI::Key::Escape;
				case Qt::Key_Space:		return CEGUI::Key::Space;
				case Qt::Key_Comma:		return CEGUI::Key::Comma;
				case Qt::Key_QuoteLeft:		return CEGUI::Key::Apostrophe;
				case Qt::Key_Minus:		return CEGUI::Key::Minus;
				case Qt::Key_Period:		return CEGUI::Key::Period;
				case Qt::Key_Slash:		return CEGUI::Key::Slash;
				case Qt::Key_Return:		return CEGUI::Key::Return;
				case Qt::Key_0:			return CEGUI::Key::Zero;
				case Qt::Key_1:			return CEGUI::Key::One;
				case Qt::Key_2:			return CEGUI::Key::Two;
				case Qt::Key_3:			return CEGUI::Key::Three;
				case Qt::Key_4:			return CEGUI::Key::Four;
				case Qt::Key_5:			return CEGUI::Key::Five;
				case Qt::Key_6:			return CEGUI::Key::Six;
				case Qt::Key_7:			return CEGUI::Key::Seven;
				case Qt::Key_8:			return CEGUI::Key::Eight;
				case Qt::Key_9:			return CEGUI::Key::Nine;
				case Qt::Key_Colon:		return CEGUI::Key::Colon;
				case Qt::Key_Semicolon:		return CEGUI::Key::Semicolon;
				case Qt::Key_Equal:		return CEGUI::Key::Equals;
				case Qt::Key_BracketLeft:	return CEGUI::Key::LeftBracket;
				case Qt::Key_Backslash:		return CEGUI::Key::Backslash;
				case Qt::Key_BracketRight:	return CEGUI::Key::RightBracket;
				case Qt::Key_A:			return CEGUI::Key::A;
				case Qt::Key_B:			return CEGUI::Key::B;
				case Qt::Key_C:			return CEGUI::Key::C;
				case Qt::Key_D:			return CEGUI::Key::D;
				case Qt::Key_E:			return CEGUI::Key::E;
				case Qt::Key_F:			return CEGUI::Key::F;
				case Qt::Key_G:			return CEGUI::Key::G;
				case Qt::Key_H:			return CEGUI::Key::H;
				case Qt::Key_I:			return CEGUI::Key::I;
				case Qt::Key_J:			return CEGUI::Key::J;
				case Qt::Key_K:			return CEGUI::Key::K;
				case Qt::Key_L:			return CEGUI::Key::L;
				case Qt::Key_M:			return CEGUI::Key::M;
				case Qt::Key_N:			return CEGUI::Key::N;
				case Qt::Key_O:			return CEGUI::Key::O;
				case Qt::Key_P:			return CEGUI::Key::P;
				case Qt::Key_Q:			return CEGUI::Key::Q;
				case Qt::Key_R:			return CEGUI::Key::R;
				case Qt::Key_S:			return CEGUI::Key::S;
				case Qt::Key_T:			return CEGUI::Key::T;
				case Qt::Key_U:			return CEGUI::Key::U;
				case Qt::Key_V:			return CEGUI::Key::V;
				case Qt::Key_W:			return CEGUI::Key::W;
				case Qt::Key_X:			return CEGUI::Key::X;
				case Qt::Key_Y:			return CEGUI::Key::Y;
				case Qt::Key_Z:			return CEGUI::Key::Z;
				case Qt::Key_Delete:		return CEGUI::Key::Delete;
				case Qt::Key_Up:		return CEGUI::Key::ArrowUp;
				case Qt::Key_Down:		return CEGUI::Key::ArrowDown;
				case Qt::Key_Right:		return CEGUI::Key::ArrowRight;
				case Qt::Key_Left:		return CEGUI::Key::ArrowLeft;
				case Qt::Key_Insert:		return CEGUI::Key::Insert;
				case Qt::Key_Home:		return CEGUI::Key::Home;
				case Qt::Key_End:		return CEGUI::Key::End;
				case Qt::Key_PageUp:		return CEGUI::Key::PageUp;
				case Qt::Key_PageDown:		return CEGUI::Key::PageDown;
				case Qt::Key_F1:		return CEGUI::Key::F1;
				case Qt::Key_F2:		return CEGUI::Key::F2;
				case Qt::Key_F3:		return CEGUI::Key::F3;
				case Qt::Key_F4:		return CEGUI::Key::F4;
				case Qt::Key_F5:		return CEGUI::Key::F5;
				case Qt::Key_F6:		return CEGUI::Key::F6;
				case Qt::Key_F7:		return CEGUI::Key::F7;
				case Qt::Key_F8:		return CEGUI::Key::F8;
				case Qt::Key_F9:		return CEGUI::Key::F9;
				case Qt::Key_F10:		return CEGUI::Key::F10;
				case Qt::Key_F11:		return CEGUI::Key::F11;
				case Qt::Key_F12:		return CEGUI::Key::F12;
				case Qt::Key_F13:		return CEGUI::Key::F13;
				case Qt::Key_F14:		return CEGUI::Key::F14;
				case Qt::Key_F15:		return CEGUI::Key::F15;
				case Qt::Key_NumLock:		return CEGUI::Key::NumLock;
				case Qt::Key_ScrollLock:	return CEGUI::Key::ScrollLock;
				case Qt::Key_Shift:		return CEGUI::Key::RightShift;
				case Qt::Key_Control:		return CEGUI::Key::RightControl;
				case Qt::Key_Alt:		return CEGUI::Key::RightAlt;
				case Qt::Key_Meta:		return CEGUI::Key::LeftWindows;
				case Qt::Key_SysReq:		return CEGUI::Key::SysRq;
				case Qt::Key_Menu:		return CEGUI::Key::AppMenu;
			}
		}
		return 0;
	}

	CEGUI::MouseButton QtCegui::convertQtToCEGUIMouseButton (Qt::MouseButton button)
	{
		switch (button)
		{
			case Qt::LeftButton:	return CEGUI::LeftButton;
			case Qt::RightButton:	return CEGUI::RightButton;
			case Qt::MidButton:		return CEGUI::MiddleButton;
			case Qt::XButton1:		return CEGUI::X1Button;
			case Qt::XButton2:		return CEGUI::X2Button;
			default:				return CEGUI::NoButton;
		}
	}



} // namespace Cnoti3D
