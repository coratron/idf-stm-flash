idf_component_register(SRCS "src/stm_flash.c" "src/stm_pro_mode.c" "src/stm_flash_http.c" "src/stm_option_bytes.c"
                       INCLUDE_DIRS "src"
                       REQUIRES esp_event esp_wifi esp_http_client esp-tls
                       			freertos spiffs nvs_flash esp_rom fatfs vfs sdmmc)
