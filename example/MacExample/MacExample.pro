######################################################################
#   Pro vars
######################################################################

PRIFILE  = MacExampleFiles.pri
LIB_MODE = shared

######################################################################
#   Include common settings
######################################################################
exists(settings.pro):include(settings.pro)
else:message("I can't find settings file")

exists (../../src/ProjectSettings_mac.pro):include(../../src/ProjectSettings_mac.pro)
else:message("Mac settings (ProjectSettings_mac) not found.")

exists (../ProjectSettings_Cnoti3D.pro):include(../ProjectSettings_Cnoti3D.pro)
else:message("Cnoti3D settings (../ProjectSettings_Cnoti3D) not found.")

######################################################################
#   Project definitions
######################################################################

TARGET = MacExample

######################################################################
#   Include pri file
######################################################################

include( $$PRIFILE )

######################################################################
