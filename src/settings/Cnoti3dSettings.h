#ifndef CNOTI3D_SETTINGS_H
#define CNOTI3D_SETTINGS_H

//Qt
#include <QString>
#include <QHash>

//class CnotiSettings;

namespace Settings
{
	// Options
	const QString YES                                     = "Yes";
	const QString NO                                      = "No";

	// Name of the settings
	const QString RENDER_SYSTEM                           = "Render System";
	// Default values for the settings
	const int     DEFAULT_SCREEN_WIDTH                    = 800;
	const int     DEFAULT_SCREEN_HEIGHT                   = 600;
	const int     DEFAULT_MONITOR                         = -1; // -1 means it will use the default screen/monitor
	const QString DEFAULT_RENDER_SYSTEM                   = "Undefined";

	// Name of the group for saving the OpenGL Ogre settings
	const QString OPENGL                                  = "OpenGL";
	const QString DIRECT3D9                               = "Direct3D9";

	// Open GL settings keys
	const QString RENDER_VIDEO_MODE                       = "Video Mode";
	const QString RENDER_FULLSCREEN                       = "Full Screen";


//	void updateOgreRenderSettings(const CnotiSettings* settings);
}

#endif // CNOTI3D_SETTINGS_H
