######################################################################
#   Pro vars
######################################################################

PRIFILE  = Cnoti3D.pri
LIB_MODE = static

######################################################################
#   Include common settings
######################################################################
exists(settings.pro):include(settings.pro)
else:message("I can't find settings file")

######################################################################
#   Project definitions
######################################################################

CONFIG += staticlib create_prl

######################################################################
#   Include pri file
######################################################################

include( $$PRIFILE )

######################################################################