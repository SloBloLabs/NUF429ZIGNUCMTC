#include "myAppMain.h"
#include "main.h"
#include <cstdio>
#include "usbd_midi_if.h"
#include "string.h"

void projectMain() {
    // Configure and enable Systick timer including interrupt
    SysTick_Config(SystemCoreClock / 1000 - 1);

    //char const *data = "Hello from NUF429ZIGNUCMTC\n";

    while(true) {
        while(!LL_GPIO_IsInputPinSet(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin)) {
          //MIDI_ProcessUSBData();

          //LL_GPIO_ResetOutputPin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
          //for(uint8_t i = 0; i < buffUsbCurrIndex; ++i) {

          //  LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
          //}
        }

        LL_GPIO_ResetOutputPin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
        for(uint8_t i = 0; i < 20; ++i) {
            LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
            uint32_t pinSet = LL_GPIO_IsOutputPinSet(GPIOB, LED_GREEN_Pin);
            printf("loop %d\n", i);
            uint8_t cable = 0;
            uint8_t message = pinSet ? 9 : 8;
            uint8_t param1 = 0x3C;
            uint8_t param2 = 0x64;
            uint8_t msg[4] = {cable, message, param1, param2};
            MIDI_sendMessage(msg, 4);
            //CDC_Transmit_FS((uint8_t*)data, strlen(data));
            LL_mDelay(200);
        }
    }
}
