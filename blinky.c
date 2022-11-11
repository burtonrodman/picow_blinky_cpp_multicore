#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"

void core1_entry() {
    uint32_t d;

    while (true) {
        d = multicore_fifo_pop_blocking();
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, d);
    }
}

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("WiFi init failed");
        return -1;
    }

    multicore_launch_core1(core1_entry);

    while (true) {
        multicore_fifo_push_blocking(1);
        sleep_ms(100);
        multicore_fifo_push_blocking(0);
        sleep_ms(100);
        multicore_fifo_push_blocking(1);
        sleep_ms(100);
        multicore_fifo_push_blocking(0);
        sleep_ms(700);
    }

    return 0;
}

