#include "myAppMain.h"
#include "main.h"
#include <cstdio>
#include "usbd_midi_if.h"
#include "string.h"
#include "midi/MidiHandler.h"

MidiHandler midiHandler;

void projectMain() {
    // Configure and enable Systick timer including interrupt
    SysTick_Config(SystemCoreClock / 1000 - 1);

    midiHandler.init();

    //char const *data = "Hello from NUF429ZIGNUCMTC\n";

    while(true) {
        while(!LL_GPIO_IsInputPinSet(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin)) {
          while(!midiHandler.incomingIsEmpty()) {
            MidiMessage msg;
            midiHandler.dequeueIncoming(&msg);
            printf("Midi Receive: ");
            MidiMessage::dump(msg);

            LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
          }
        }

        LL_GPIO_ResetOutputPin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
        for(uint8_t i = 0; i < 2; ++i) {
            LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
            uint32_t pinSet = LL_GPIO_IsOutputPinSet(GPIOB, LED_GREEN_Pin);
            //printf("loop %d\n", i);
            uint8_t midiChannel = 5;
            MidiMessage msg = pinSet ?
                MidiMessage::makeNoteOn(midiChannel, 0x3C, 100) :
                MidiMessage::makeNoteOff(midiChannel, 0x3C, 100);
            printf("Midi Send: ");
            MidiMessage::dump(msg);
            midiHandler.enqueueOutgoing(msg);
            midiHandler.processOutgoing();
            //CDC_Transmit_FS((uint8_t*)data, strlen(data));
            LL_mDelay(100);
        }
    }
}

extern "C" {

void enqueueIncoming(uint8_t *data) {
    MidiUSBMessage umsg(data);
    MidiMessage msg;
    umsg.getMidiMessage(msg);
    midiHandler.enqueueIncoming(msg);
}

}