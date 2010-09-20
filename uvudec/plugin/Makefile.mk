# Plugin boiler plate makefile
# UVNet Universal Decompiler (uvudec)
# Copyright 2010 John McMaster <JohnDMcMaster@gmail.com>
# Licensed under terms of the three clause BSD license, see LICENSE for details

# For static builds, we statically link the plugin
# For dynamic builds, we 

# DEFINES += -DUVDPluginMain=UVDPluginMain$(PLUGIN_NAME)

# TODO: substitute chars for underscores?
# We wrap this with PLUGIN_NAME since current dur uses it as well
ifdef PLUGIN_NAME
ifndef LIB_NAME
LIB_NAME=$(PLUGIN_NAME)
endif
endif

THIS_LIB_DIR=$(PLUGIN_LIB_DIR)
# Creates duplicate plugin loading
CREATE_LIB_LINKS=N

include $(ROOT_DIR)/Makefile.mk

# UVD official plugins should prefix include paths with the plugin name
INCLUDES += -I$(PLUGIN_DIR)

