#include "Fader.h"
// Ogre
#include <OgreMaterialManager.h>
#include <OgreOverlayManager.h>
#include <OgreTechnique.h>
#include <OgreBlendMode.h>
// Qt
#include <QDebug>

// Source: http://www.ogre3d.org/tikiwiki/FadeEffectOverlay&structure=Cookbook

namespace Cnoti3D
{
 Fader::Fader(const char *OverlayName, const char *MaterialName, FaderCallback *instance)
 {
	 try
	 {
		 _fadeop = FADE_NONE;
		 _alpha = 0.0;
		 _inst = instance;

		 // Get the material by name
		 Ogre::ResourcePtr resptr = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
		 Ogre::Material * mat = dynamic_cast<Ogre::Material*>(resptr.getPointer());

		 Ogre::Technique *tech = mat->getTechnique(0);    // Get the technique
		 Ogre::Pass *pass = tech->getPass(0);            // Get the pass
		 if (_tex_unit)
		 {
			_tex_unit = pass->getTextureUnitState(0);        // Get the texture_unit state
		 }
		 else
		 {
			 qWarning() << "[Fader::Fader] Tex unit not initialized. Fade not available";
		 }

		 // Get the _overlay
		 _overlay = Ogre::OverlayManager::getSingleton().getByName(OverlayName);
		 if (_overlay != NULL)
		 {
			 _overlay->hide();
		 }
		 else
		 {
			 qWarning() << "[Fader::Fader] Overlay not initialized. Fade not available";
		 }

	 } catch(Ogre::Exception e) {
		 qDebug() << "[Fader::Fader] Fader Exception:" << e.getFullDescription().c_str();
	 } catch(...) {
		 qDebug() << "[Fader::Fader] Fader Exception: An unknown exception has occured while setting up the fader.  Scene fading will not be supported.";
	 }
 }

 Fader::~Fader(void)
 {
 }

 void Fader::startFadeIn(double duration, bool force)
 {
	 if (_overlay == NULL || (_alpha <= 0.0 && !force) )
	 {
		 qWarning() << "[Fader::startFadeIn] Fade in is not going to be done. _overlay is NULL or _aplha already at end value.";
		 return;
	 }

	 if( duration < 0 )
		 duration = -duration;
	 if( duration < 0.000001 )
		 duration = 1.0;

	 _alpha = 1.0;
	 _total_dur = duration;
	 _current_dur = duration;
	 _fadeop = FADE_IN;
	 _overlay->show();
 }

 void Fader::startFadeOut(double duration, bool force)
 {
	 if (_overlay == NULL || (_alpha >= 1.0 && !force) )
	 {
		 qWarning() << "[Fader::startFadeOut] Fade in is not going to be done. _overlay is NULL or _aplha already at end value.";
		 return;
	 }

	 if( duration < 0 )
		 duration = -duration;
	 if( duration < 0.000001 )
		 duration = 1.0;

	 _alpha = 0.0;
	 _total_dur = duration;
	 _current_dur = 0.0;
	 _fadeop = FADE_OUT;
	 _overlay->show();
 }

 void Fader::fade(double timeSinceLastFrame)
 {
	if( _fadeop != FADE_NONE && _tex_unit )
	{
		 // Set the _alpha value of the _overlay
		 _tex_unit->setAlphaOperation(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_TEXTURE, _alpha);    // Change the _alpha operation

		 // If fading in, decrease the _alpha until it reaches 0.0
		 if( _fadeop == FADE_IN )
		 {
			 _current_dur -= timeSinceLastFrame;
			 _alpha = _current_dur / _total_dur;
//			 qDebug() << "[Fader::fade]" << _alpha << timeSinceLastFrame << _current_dur << _total_dur;
			 if( _alpha < 0.0 )
			 {
				 _overlay->hide();
				 _fadeop = FADE_NONE;
				 if( _inst )
					 _inst->fadeInCallback();
				 _alpha = 0.0f;
			 }
		 }

		 // If fading out, increase the _alpha until it reaches 1.0
		 else if( _fadeop == FADE_OUT )
		 {
			 _current_dur += timeSinceLastFrame;
			 _alpha = _current_dur / _total_dur;
			 if( _alpha > 1.0 )
			 {
				 _fadeop = FADE_NONE;
				 if( _inst )
					 _inst->fadeOutCallback();
				 _alpha = 1.0f;
			 }
		 }
	}
	else if(_tex_unit == NULL)
	{
		if( _fadeop == FADE_IN )
		{
			_alpha = 0.0f;
			_overlay->hide();
		}
		else if( _fadeop == FADE_OUT )
		{
			 _alpha = 1.0f;
			 _overlay->show();
		}
	}

 }

} // namespace Cnoti3D
