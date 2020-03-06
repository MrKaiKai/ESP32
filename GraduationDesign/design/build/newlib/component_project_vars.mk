# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/components/newlib/platform_include $(PROJECT_PATH)/components/newlib/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/newlib $(PROJECT_PATH)/components/newlib/lib/libc-psram-workaround.a $(PROJECT_PATH)/components/newlib/lib/libm-psram-workaround.a -lnewlib
COMPONENT_LINKER_DEPS += $(PROJECT_PATH)/components/newlib/lib/libc-psram-workaround.a $(PROJECT_PATH)/components/newlib/lib/libm-psram-workaround.a
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += newlib
component-newlib-build: 
