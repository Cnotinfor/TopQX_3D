######################################################################
#
######################################################################

TEMPLATE = app

CONFIG += app_bundle

QT += opengl


#####################################################################
# FOLDER OF THIS PROJECT ON WE HAVE (src, static, static, docs )
# IF YOU DON'T HAVE SUB PROJECTS IS ONLY "ROOT_PROJECT_FOLDER = .."
ROOT_PROJECT_FOLDER = ../MacExample

# ###################################################################
# DEFAULT CONFIGURATIONS
# ###################################################################

CONFIG( debug, debug|release ) {
	TARGET = $${TARGET}_d
	BUILD_NAME = debug
	DESTDIR = $${ROOT_PROJECT_FOLDER}/../debug
}
CONFIG( release, debug|release ) {
	BUILD_NAME = release
	DESTDIR = $${ROOT_PROJECT_FOLDER}/../release
}

