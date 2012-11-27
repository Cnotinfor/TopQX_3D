# -------------------------
#
# Project to work with Ogre SDK 1.7.2 for Visual Studio 2008 (VC9)
#
# -------------------------

TARGET = Cnoti3D

TEMPLATE = lib
DEPENDPATH += .

INCLUDEPATH += . \
			   ./oSceneLoader/include

win32 {
INCLUDEPATH += $(OGRE_HOME)/include/OGRE \
                           $(OGRE_HOME)/boost_1_44 \
			   $(CEGUI_HOME)/cegui/include
}

mac {
	exists (ProjectSettings_mac.pro):include(ProjectSettings_mac.pro)
	else:message("Mac settings (ProjectSettings_mac) not found.")
}

#####################################################################
# FOLDER OF THIS PROJECT ON WE HAVE (src, static, static, docs )
# IF YOU DON'T HAVE SUB PROJECTS IS ONLY "ROOT_PROJECT_FOLDER = .."
ROOT_PROJECT_FOLDER = ..

# ###################################################################
# DEFAULT CONFIGURATIONS
# ###################################################################

DESTDIR = $${ROOT_PROJECT_FOLDER}/lib/shared

DEFINES *= \
    CNOTI_SHARED_LIB \
    CNOTI3DLIB_LIB

win32 {
        LIBS += -L$(OGRE_HOME)/boost_1_44/lib \
			-L$${ROOT_PROJECT_FOLDER}/3rdparty/lib \
			-L$(CEGUI_HOME)/lib

	CONFIG( debug, debug|release ) {
		TARGET = $${TARGET}_d
		BUILD_NAME = debug
		LIBS += -L$(OGRE_HOME)/lib/debug \
				-lOgreMain_d \
                                -llibboost_thread-vc90-mt-gd-1_44 \
				-lCEGUIBase_d \
				-lCEGUIOgreRenderer_d
	}
	CONFIG( release, debug|release ) {
		BUILD_NAME = release
		LIBS += -L$(OGRE_HOME)/lib/release \
				-lOgreMain \
                                -llibboost_thread-vc90-mt-1_44 \
				-lCEGUIBase \
				-lCEGUIOgreRenderer
	}
}

# #####################################################################
# Include pri file
# #####################################################################
include(files.pri)
include(interface/interface.pri)
include(settings/settings.pri)
include(PathFinder/PathFinder.pri)
include(3rdParty/3rdParty.pri)
