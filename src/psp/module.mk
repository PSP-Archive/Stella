MODULE := src/psp

MODULE_OBJS := \
	src/psp/FSNodePSP.o \
	src/psp/OSystemPSP.o \
	src/psp/SettingsPSP.o \
	src/psp/FrameBufferPSP.o \
	src/psp/FrameBufferPSP2.o

MODULE_DIRS += \
        src/psp

# Include common rules
include $(srcdir)/common.rules
