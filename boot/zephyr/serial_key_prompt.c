#include <stdio.h>
#include <zephyr/drivers/uart.h>
#include <assert.h>
#include <string.h>
#include <zephyr/kernel.h>
#include "bootutil/bootutil_log.h"

BOOT_LOG_MODULE_REGISTER(serial_key_prompt);

static struct device const *uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

bool boot_uart_key_prompt(void)
{
    uint32_t key_wait_start = k_uptime_get_32();
    char boot_key = 0;
    int boot_key_cnt = 0;
    do {
        if (uart_poll_in(uart_dev, (uint8_t*)&boot_key) == 0) {
            if (boot_key == 'b') {
                boot_key_cnt++;
            } else {
                boot_key_cnt = 0;
            }
            if (boot_key_cnt >= 3) {
                BOOT_LOG_INF("Bootloader key prompted");
                return true;
            }
        }
    } while ((k_uptime_get_32() - key_wait_start) < 10000U);

    return false;
}
