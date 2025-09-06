#include "esp_log.h"
#include "nvs_flash.h"
/*Data communication*/
#include "driver/uart.h"   
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/*Servo control*/
#include "servo.h"
#include "driver/ledc.h"

/*Define UART pin*/
#define TXD_PIN (GPIO_NUM_17)  // ESP -> nRF 


void uart_send_task(void *pvParameters)
{
    uint8_t high, low;
    const int step = 1;        
    const int delay_ms = 20;   

    while (1) {
        if (uart_read_bytes(UART_NUM_2, &high, 1, pdMS_TO_TICKS(100)) > 0 &&
            uart_read_bytes(UART_NUM_2, &low, 1, pdMS_TO_TICKS(100)) > 0) {

            uint16_t value = ((uint16_t)high << 8) | low;

            if (value > 450) {
                // Send On-Data to Earrings
                uint8_t sendByte = '1';  
                uart_write_bytes(UART_NUM_1, (const char*)&sendByte, 1);

                // 90 → 150
                for (int angle = 90; angle <= 180; angle += step) {
                    servo_set_angle(angle);
                    vTaskDelay(pdMS_TO_TICKS(delay_ms));
                }
                // 150 → 90
                for (int angle = 180; angle >= 90; angle -= step) {
                    servo_set_angle(angle);
                    vTaskDelay(pdMS_TO_TICKS(delay_ms));
                }

            } else {
                // Send Off-Data to Earrings
                uint8_t sendByte = '0'; 
                uart_write_bytes(UART_NUM_1, (const char*)&sendByte, 1);

                // return to 0
                //servo_set_angle(0);
            }
        }
    }
}



void uart_init(void)
{   // Scarf(EPS32) -> Earrings(nRF52)
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_driver_install(UART_NUM_1, 1024, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

   // Anemometer(ESP32) -> Scarf(ESP32)
    uart_config_t uart_config2 = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity    = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_driver_install(UART_NUM_2, 1024, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_2, &uart_config2);
    uart_set_pin(UART_NUM_2, UART_PIN_NO_CHANGE, GPIO_NUM_22, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

}





void app_main(void){

    /*-----UART send data------*/
    uart_init();
    xTaskCreate(uart_send_task, "uart_send_task", 2048, NULL, 5, NULL);   // <- back

    /*Servo setting*/
    servo_init();
    servo_set_angle(90);

}
