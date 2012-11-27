@echo off

set COPY_COMMAND=xcopy /D /Y

if "%1" == "release" goto RELEASE
if "%1" == "debug" goto DEBUG
if "%1" == "" goto HELP


:RELEASE
set UP_MODE=Release
set UP_DEST_FOLDER="..\release"
set UP_CNOTI3D_FILE="%CNOTI3D_HOME%\lib\shared\Cnoti3D.dll"
set UP_OGRE_FILE1="%OGRE_HOME%\bin\release\OgreMain.dll" 
set UP_OGRE_FILE2="%OGRE_HOME%\bin\release\RenderSystem_GL.dll" 

goto COPY


:DEBUG
set UP_MODE=Debug
set UP_DEST_FOLDER="..\debug"
set UP_CNOTI3D_FILE="%CNOTI3D_HOME%\lib\shared\Cnoti3D_d.dll"
set UP_OGRE_FILE1="%OGRE_HOME%\bin\debug\OgreMain_d.dll" 
set UP_OGRE_FILE2="%OGRE_HOME%\bin\debug\RenderSystem_GL_d.dll" 

goto COPY


:COPY
echo %UP_MODE% Mode

echo Updating Cnoti 3D DLL
%COPY_COMMAND% %UP_CNOTI3D_FILE% %UP_DEST_FOLDER%

echo Updating Ogre dlls
%COPY_COMMAND% %UP_OGRE_FILE1% %UP_DEST_FOLDER%
%COPY_COMMAND% %UP_OGRE_FILE2% %UP_DEST_FOLDER%

goto END


: HELP
echo To copy the libraries choose between debug and release
echo update_libs debug,release


:END


set UP_CNOTI3D_FILE=
set UP_SOUNDMANAGER_FILE=
set UP_DEST_FOLDER=
set UP_MODE=
set UP_QT_FLAG=
set UP_OGRE_FILE1=
set UP_OGRE_FILE2=
set COPY_COMMAND=