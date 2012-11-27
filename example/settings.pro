######################################################################
#
######################################################################

TEMPLATE = app

QT += opengl

win32 {
INCLUDEPATH += $(OGRE_HOME)/include/Ogre \
			   $(OGRE_HOME)/boost_1_44 \
			   $(CEGUI_HOME)/cegui/include \
			   ../../include
}
#####################################################################
# FOLDER OF THIS PROJECT ON WE HAVE (src, static, static, docs )
# IF YOU DON'T HAVE SUB PROJECTS IS ONLY "ROOT_PROJECT_FOLDER = .."
ROOT_PROJECT_FOLDER = ../

# ###################################################################
# DEFAULT CONFIGURATIONS
# ###################################################################

win32 {
	LIBS += -L$(OGRE_HOME)/boost_1_44/lib \
			-llibboost_thread-vc90-mt-gd-1_44 \
			-L$${ROOT_PROJECT_FOLDER}/3rdparty/lib \
			-L$(CNOTI3D_HOME)/lib/shared \
			-L$(CEGUI_HOME)/lib

    CONFIG( debug, debug|release ) {
        TARGET = $${TARGET}_d
        BUILD_NAME = debug
    #	DEFINES += QT_DEBUG
        LIBS += -L$(OGRE_HOME)/lib/debug \
				-lOgreMain_d \
				-lCnoti3D_d \
				-lCEGUIBase_d \
				-lCEGUIOgreRenderer_d
        DESTDIR = $${ROOT_PROJECT_FOLDER}/debug
    }
    CONFIG( release, debug|release ) {
        BUILD_NAME = release
        LIBS += -L$(OGRE_HOME)/lib/release \
				-lOgreMain \
				-lCnoti3D \
				-lCEGUIBase \
				-lCEGUIOgreRenderer
        DESTDIR = $${ROOT_PROJECT_FOLDER}/release
    }
}

macx {
	CONFIG( debug, debug|release ) {
		DESTDIR = $${ROOT_PROJECT_FOLDER}/debug
	}
	CONFIG( release, debug|release ) {
		DESTDIR = $${ROOT_PROJECT_FOLDER}/release
	}
}


