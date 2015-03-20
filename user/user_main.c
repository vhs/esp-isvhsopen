#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_interface.h"
#include "wifi.h"

#define INTERVAL 60000
#define VHS_OPEN_LED_PIN 12
#define VHS_CLOSED_LED_PIN 13
#define HIGH 1
#define LOW 0
#define WIFI_SSID "TODO"
#define WIFI_PASSWORD "TODO"

LOCAL os_timer_t poll_timer;

LOCAL void http_callback(char * response, int http_status, char * full_response) {
  os_printf("HTTP %d: %s\r\n", http_status, response);

  if (strcmp(response, "open") == 0) {
    GPIO_OUTPUT_SET(VHS_OPEN_LED_PIN, HIGH);
    GPIO_OUTPUT_SET(VHS_CLOSED_LED_PIN, LOW);
  } else {
    GPIO_OUTPUT_SET(VHS_OPEN_LED_PIN, LOW);
    GPIO_OUTPUT_SET(VHS_CLOSED_LED_PIN, HIGH);
  }
}

LOCAL void ICACHE_FLASH_ATTR poll_callback(void *arg) {
  os_printf("Is VHS Open?\r\n");
  http_get("http://api.hackspace.ca/s/vhs/data/door.txt", http_callback);
}

LOCAL void wifiConnectCb(uint8_t status) {
  if(status == STATION_GOT_IP){
    os_printf("Connected to wifi\r\n");

    // Set up a callback function to be called every INTERVAL
    os_timer_disarm(&poll_timer);
    os_timer_setfn(&poll_timer, (os_timer_func_t *)poll_callback, (void *)0);
    os_timer_arm(&poll_timer, INTERVAL, 1);

    // Call it once right away
    poll_callback(0);

  } else {
    os_printf("Could not connect to wifi\r\n");
  }
}

void ICACHE_FLASH_ATTR user_init() {
  // Initialize UART0
  uart_div_modify(0, UART_CLK_FREQ / 115200);

  // Initialize GPIO subsystem
  gpio_init();

  // Set pins 12 & 13 to be used for regular output
  // http://esp8266.ru/esp8266-pin-register-strapping/
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);

  // and turn on both LEDs to show we're alive
  GPIO_OUTPUT_SET(VHS_OPEN_LED_PIN, HIGH);
  GPIO_OUTPUT_SET(VHS_CLOSED_LED_PIN, HIGH);

  WIFI_Connect(WIFI_SSID, WIFI_PASSWORD, wifiConnectCb);

  // Print something to UART0
  os_printf("Initialized\r\n");
}
