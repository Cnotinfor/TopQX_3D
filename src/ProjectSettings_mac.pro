#----------------------------------------------------------------------
# Include paths
#----------------------------------------------------------------------

INCLUDEPATH +=  /Library/OgreSDK/include/OGRE \
                /Library/OgreSDK/include/OGRE/RenderSystems/GL \
                /Library/OgreSDK/boost_1_46_1 \
				/Work/Libs/CEGUI/CEGUI-0.7.5/cegui/include \
				/Work/Libs/CEGUI/CEGUI-0.7.5/cegui/src/implementations/mac

#----------------------------------------------------------------------
# Libs
#----------------------------------------------------------------------

QMAKE_LFLAGS += -F/System/Library/Frameworks \
                -F/Library/OgreSDK/lib/release \
                -F/Work/Libs/CEGUI/CEGUI-0.7.5/projects/Xcode/build/Release

LIBS += -framework Ogre \
        -framework CEGUIBase \
        -framework CEGUIOgreRenderer

CONFIG( debug, debug|release ) {
        TARGET = $${TARGET}_d
        BUILD_NAME = debug

}
CONFIG( release, debug|release ) {
        BUILD_NAME = release
}
