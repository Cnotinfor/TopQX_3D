######################################################################
#
######################################################################

TARGET = Cnoti3D

TEMPLATE = lib
DEPENDPATH += .
INCLUDEPATH += . \
			   ./oSceneLoader/include \
			   3rdparty/micropather_1_2_0 \
			   $(OGRE_HOME)/include \
			   $(OGRE_HOME)/OgreMain/include \
			   $(CEGUI_HOME)/cegui/include

QT *= opengl
#	  declarative

#####################################################################
# FOLDER OF THIS PROJECT ON WE HAVE (src, static, static, docs )
# IF YOU DON'T HAVE SUB PROJECTS IS ONLY "ROOT_PROJECT_FOLDER = .."
ROOT_PROJECT_FOLDER = ..

# ###################################################################
# DEFAULT CONFIGURATIONS
# ###################################################################

DESTDIR = $${ROOT_PROJECT_FOLDER}/lib/shared

DEFINES *= CNOTI_SHARED_LIB \
		   CNOTI3DLIB_LIB

LIBS += -L$(CEGUI_HOME)/lib

CONFIG( debug, debug|release ) {
	TARGET = $${TARGET}_d
	BUILD_NAME = debug
	LIBS += -L$(OGRE_HOME)/lib/Debug \
			-lOgreMain_d \
			-lCEGUIBase_d \
			-lCEGUIOgreRenderer_d
}
CONFIG( release, debug|release ) {
	BUILD_NAME = release
	LIBS += -L$(OGRE_HOME)/lib/Release \
			-lOgreMain \
			-lCEGUIBase \
			-lCEGUIOgreRenderer
}

INCLUDEPATH += $${ROOT_PROJECT_FOLDER}/tmp/shared/$$BUILD_NAME/GeneratedFiles \
    $${ROOT_PROJECT_FOLDER}/tmp/shared/GeneratedFiles
OBJECTS_DIR += $${ROOT_PROJECT_FOLDER}/tmp/shared/$$BUILD_NAME/GeneratedFiles
MOC_DIR += $${ROOT_PROJECT_FOLDER}/tmp/shared/$$BUILD_NAME/GeneratedFiles
UI_DIR += $${ROOT_PROJECT_FOLDER}/tmp/shared/GeneratedFiles
RCC_DIR += $${ROOT_PROJECT_FOLDER}/tmp/shared/GeneratedFiles


# #####################################################################
# Include pri file
# #####################################################################
PRIFILE  = files.pri
include( $$PRIFILE )
include(interface/interface.pri)
