#!/bin/sh

APP_NAME="$2"
FRAMEWORK_FOLDER="../debug/$APP_NAME.app/Contents/Frameworks"
RESOURCES_FOLDER="../debug/$APP_NAME.app/Contents/Resources"
EXECUTABLE_FOLDER="../debug/$APP_NAME.app/Contents/MacOS"
PLUGINS_FOLDER="../debug/$APP_NAME.app/Contents/Plugins"
#PLUGINS_FOLDER=$EXECUTABLE_FOLDER

#if [ ! -d "$FRAMEWORK_FOLDER" ]; then
if [ -d "$FRAMEWORK_FOLDER" ]; then
	rm -R $FRAMEWORK_FOLDER
fi
mkdir $FRAMEWORK_FOLDER

# Copy resources
cp root_dir_files/resources.cfg $EXECUTABLE_FOLDER/resources_d.cfg
cp -R ../Resources/* $RESOURCES_FOLDER
#cp ../Resources/Batutas/*.* $RESOURCES_FOLDER
#cp ../Resources/House/*.* $RESOURCES_FOLDER
#cp ../Resources/gui/*.* $RESOURCES_FOLDER
#cp ../Resources/gui/fonts/*.* $RESOURCES_FOLDER

# Copy plugins
if [ -d "$PLUGINS_FOLDER" ]; then
	rm -R $PLUGINS_FOLDER
fi
mkdir $PLUGINS_FOLDER

cp /Library/OgreSDK/lib/RenderSystem_GL.dylib $PLUGINS_FOLDER

# Copy libraries
cp -R /Library/OgreSDK/lib/release/Ogre.framework $FRAMEWORK_FOLDER

#cp /Work/2010/Cnoti3D/project/lib/shared/libCnoti3D_d.1.0.0.dylib $FRAMEWORK_FOLDER/libCnoti3D_d.dylib
cp /Work/2010/Cnoti3D/project/lib/shared/libCnoti3D_d.1.0.0.dylib $EXECUTABLE_FOLDER/libCnoti3D_d.dylib

####################
#    CEGUI LIBS    #
####################
cp -R /Work/Libs/CEGUI/CEGUI-0.7.5/projects/Xcode/build/Release/CEGUIBase.framework $FRAMEWORK_FOLDER
cp -R /Work/Libs/CEGUI/CEGUI-0.7.5/projects/Xcode/build/Release/CEGUIOgreRenderer.framework $FRAMEWORK_FOLDER
cp -R /Work/Libs/CEGUI/CEGUI-0.7.5/projects/Xcode/build/Release/CEGUIExpatParser.bundle $PLUGINS_FOLDER
cp -R /Work/Libs/CEGUI/CEGUI-0.7.5/projects/Xcode/build/Release/CEGUIFalagardWRBase.bundle $PLUGINS_FOLDER


# Update paths in exe
cd $EXECUTABLE_FOLDER
#install_name_tool -change libCnoti3D_d.1.dylib @executable_path/../Frameworks/libCnoti3D_d.dylib $APP_NAME
install_name_tool -change libCnoti3D_d.1.dylib @executable_path/libCnoti3D_d.dylib $APP_NAME

