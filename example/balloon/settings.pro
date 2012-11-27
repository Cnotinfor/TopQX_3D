######################################################################
#
######################################################################

TEMPLATE = app

QT += opengl

INCLUDEPATH += $(OGRE_HOME)/OgreMain/include \
			   $(OGRE_HOME)/Builds/include \
			   ../../include

#####################################################################
# FOLDER OF THIS PROJECT ON WE HAVE (src, static, static, docs )
# IF YOU DON'T HAVE SUB PROJECTS IS ONLY "ROOT_PROJECT_FOLDER = .."
ROOT_PROJECT_FOLDER = ../balloon

# ###################################################################
# DEFAULT CONFIGURATIONS
# ###################################################################

CONFIG( debug, debug|release ) {
	TARGET = $${TARGET}_d
	BUILD_NAME = debug
		LIBS += -L$(OGRE_HOME)/Builds/lib/Debug \
				-lOgreMain_d \
				-L$(CNOTI3D_HOME)/lib/shared \
				-lCnoti3D_d
	DESTDIR = $${ROOT_PROJECT_FOLDER}/../bin/debug
}
CONFIG( release, debug|release ) {
	BUILD_NAME = release
	LIBS += -L$(OGRE_HOME)/Builds/lib/Release \
			-lOgreMain \
			-L$(CNOTI3D_HOME)/lib/shared \
			-lCnoti3D
	DESTDIR = $${ROOT_PROJECT_FOLDER}/../bin/release
}

INCLUDEPATH += $${ROOT_PROJECT_FOLDER}/tmp/$${LIB_MODE}/$${BUILD_NAME}/GeneratedFiles \
			$${ROOT_PROJECT_FOLDER}/tmp/$${LIB_MODE}/GeneratedFiles
OBJECTS_DIR += $${ROOT_PROJECT_FOLDER}/tmp/$${LIB_MODE}/$${BUILD_NAME}/GeneratedFiles
MOC_DIR += $${ROOT_PROJECT_FOLDER}/tmp/$${LIB_MODE}/$${BUILD_NAME}/GeneratedFiles
UI_DIR += $${ROOT_PROJECT_FOLDER}/tmp/$${LIB_MODE}/GeneratedFiles
RCC_DIR += $${ROOT_PROJECT_FOLDER}/tmp/$${LIB_MODE}/GeneratedFiles

