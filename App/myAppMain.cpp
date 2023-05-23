#include "myAppMain.h"
#include "main.h"
#include <cstdio>
#include "usbd_audio_if.h"
#include "string.h"

void projectMain() {
    // Configure and enable Systick timer including interrupt
    SysTick_Config(SystemCoreClock / 1000 - 1);

    //char const *data = "Hello from NUF429ZIGNUCMTC\n";

    while(true) {
        /*while(!LL_GPIO_IsInputPinSet(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin));

        for(uint8_t i = 0; i < 20; ++i) {
            LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
            printf("loop %d\n", i);
            //CDC_Transmit_FS((uint8_t*)data, strlen(data));
            LL_mDelay(50);
        }*/
    }
}
