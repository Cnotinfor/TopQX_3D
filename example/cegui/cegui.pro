######################################################################
#   Pro vars
######################################################################

PRIFILE  = cegui.pri
LIB_MODE = shared

win32{
    INCLUDEPATH += $(CEGUI_HOME)/cegui/include

    LIBS += -L$(CEGUI_HOME)/lib

    CONFIG( debug, debug|release ) {
        LIBS += -lCEGUIBase_d \
                -lCEGUIOgreRenderer_d
    }
    CONFIG( release, debug|release ) {
        LIBS += -lCEGUIBase \
                -lCEGUIOgreRenderer
    }
}

######################################################################
#   Project definitions
######################################################################

TARGET = cegui

######################################################################
#   Include common settings
######################################################################
exists(../settings.pro):include(../settings.pro)
else:message("I can't find ../settings file")

exists (../../src/ProjectSettings_mac.pro):include(../../src/ProjectSettings_mac.pro)
else:message("Mac settings (ProjectSettings_mac) not found.")

exists (../ProjectSettings_Cnoti3D.pro):include(../ProjectSettings_Cnoti3D.pro)
else:message("Cnoti3D settings (../ProjectSettings_Cnoti3D) not found.")

######################################################################
#   Include pri file
######################################################################

include( $$PRIFILE )

######################################################################
