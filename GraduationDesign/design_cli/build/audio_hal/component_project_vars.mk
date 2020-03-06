# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/components/audio_hal/include $(PROJECT_PATH)/components/audio_hal/driver/es8388 $(PROJECT_PATH)/components/audio_hal/board $(PROJECT_PATH)/components/audio_hal/driver/es8374 $(PROJECT_PATH)/components/audio_hal/driver/zl38063 $(PROJECT_PATH)/components/audio_hal/driver/zl38063/api_lib $(PROJECT_PATH)/components/audio_hal/driver/zl38063/example_apps $(PROJECT_PATH)/components/audio_hal/driver/zl38063/firmware
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/audio_hal -laudio_hal -L$(PROJECT_PATH)/components/audio_hal/driver/zl38063/firmware -lfirmware
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += audio_hal
component-audio_hal-build: 
