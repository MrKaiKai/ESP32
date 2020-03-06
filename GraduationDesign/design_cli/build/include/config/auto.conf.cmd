deps_config := \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/app_trace/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/aws_iot/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/bt/Kconfig \
	/home/ununtur6/workspace/LIU_test/esp32_learn/design_cli/components/driver/Kconfig \
	/home/ununtur6/workspace/LIU_test/esp32_learn/design_cli/components/esp32/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/esp_adc_cal/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/components/esp_http_client/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/ethernet/Kconfig \
	/home/ununtur6/workspace/LIU_test/esp32_learn/design_cli/components/fatfs/Kconfig \
	/home/ununtur6/workspace/LIU_test/esp32_learn/design_cli/components/freertos/Kconfig \
	/home/ununtur6/workspace/LIU_test/esp32_learn/design_cli/components/heap/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/components/http_server/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/libsodium/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/log/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/lwip/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/mbedtls/Kconfig \
	/home/ununtur6/workspace/LIU_test/esp32_learn/design_cli/components/nvs_flash/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/openssl/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/pthread/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/spi_flash/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/spiffs/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/tcpip_adapter/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/vfs/Kconfig \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/wear_levelling/Kconfig \
	/home/ununtur6/workspace/LIU_test/esp32_learn/design_cli/components/audio_hal/Kconfig.projbuild \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/ununtur6/workspace/LIU_test/esp32_learn/design_cli/components/esp-adf-libs/Kconfig.projbuild \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/ununtur6/workspace/LIU_test/esp32_learn/design_cli/components/partition_table/Kconfig.projbuild \
	/home/ununtur6/workspace/ESP32/esp-adf/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
