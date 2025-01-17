MODULE := engines/groovie

MODULE_OBJS := \
	cell.o \
	cursor.o \
	debug.o \
	font.o \
	graphics.o \
	groovie.o \
	lzss.o \
	metaengine.o \
	music.o \
	player.o \
	resource.o \
	saveload.o \
	script.o \
	vdx.o

ifdef ENABLE_GROOVIE2
MODULE_OBJS += \
	clangame.o \
	roq.o \
	t11hgame.o \
	tlcgame.o
endif

# This module can be built as a plugin
ifeq ($(ENABLE_GROOVIE), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
