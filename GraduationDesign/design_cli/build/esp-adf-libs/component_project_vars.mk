# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/components/esp-adf-libs/esp_audio/include $(PROJECT_PATH)/components/esp-adf-libs/esp_codec/include/codec $(PROJECT_PATH)/components/esp-adf-libs/esp_codec/include/processing $(PROJECT_PATH)/components/esp-adf-libs/recorder_engine/include $(PROJECT_PATH)/components/esp-adf-libs/esp_sr/include $(PROJECT_PATH)/components/esp-adf-libs/esp_ssdp/include $(PROJECT_PATH)/components/esp-adf-libs/esp_dlna/include $(PROJECT_PATH)/components/esp-adf-libs/esp_upnp/include
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/esp-adf-libs -lesp-adf-libs -L$(PROJECT_PATH)/components/esp-adf-libs/esp_audio/lib -L$(PROJECT_PATH)/components/esp-adf-libs/esp_codec/lib -L$(PROJECT_PATH)/components/esp-adf-libs/recorder_engine/lib -L$(PROJECT_PATH)/components/esp-adf-libs/esp_sr/lib -L$(PROJECT_PATH)/components/esp-adf-libs/esp_ssdp/lib -L$(PROJECT_PATH)/components/esp-adf-libs/esp_upnp/lib -L$(PROJECT_PATH)/components/esp-adf-libs/esp_dlna/lib -lesp_codec -lesp_audio -lesp-amr -lesp-amrwbenc -lesp-aac -lesp-ogg-container -lesp-opus -lesp-tremor -lesp-flac -lrecorder_engine -lesp_ssdp -lesp_upnp -lesp_dlna -lvad -lesp_wakenet -lnn_model_light_control_ch_wn4 
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += esp-adf-libs
component-esp-adf-libs-build: 
