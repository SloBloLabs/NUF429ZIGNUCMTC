#include "myAppMain.h"
#include "main.h"
#include <cstdio>
#include "usbd_cdc_if.h"
#include "usbd_midi_if.h"
#include "string.h"
#include "midi/MidiHandler.h"

MidiHandler midiHandler;

void cdcTest();
void midiTest();

void projectMain() {
    // Configure and enable Systick timer including interrupt
    SysTick_Config(SystemCoreClock / 1000 - 1);

    //cdcTest();
    midiTest();

}

void cdcTest() {
    char const *data = "Hello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\nHello from NUF429ZIGNUCMTC\n";

    while(true) {
        while(!LL_GPIO_IsInputPinSet(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin));

        CDC_Transmit_FS((uint8_t*)data, strlen(data));
        LL_mDelay(100);
    }
}

void midiTest() {

    midiHandler.init();

    while(true) {
        while(!LL_GPIO_IsInputPinSet(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin)) {

          MidiMessage msg;
          while(midiHandler.dequeueIncoming(&msg)) {
            printf("Midi Receive: ");
            MidiMessage::dump(msg);

            LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
          }
        }

        uint8_t midiChannel = 15;
        uint8_t timeSpan = 200;

        char const *data = "Play Melody!\n";
        CDC_Transmit_FS((uint8_t*)data, strlen(data));

        MidiMessage msg;
        msg = MidiMessage::makeNoteOn(midiChannel, 0x3C, 100);
        midiHandler.enqueueOutgoing(msg);
        midiHandler.processOutgoing();
        LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
        LL_mDelay(timeSpan);
        msg = MidiMessage::makeNoteOff(midiChannel, 0x3C, 100);
        midiHandler.enqueueOutgoing(msg);
        msg = MidiMessage::makeNoteOn(midiChannel, 0x3E, 100);
        midiHandler.enqueueOutgoing(msg);
        midiHandler.processOutgoing();
        LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
        LL_mDelay(timeSpan);
        msg = MidiMessage::makeNoteOff(midiChannel, 0x3E, 100);
        midiHandler.enqueueOutgoing(msg);
        msg = MidiMessage::makeNoteOn(midiChannel, 0x40, 100);
        midiHandler.enqueueOutgoing(msg);
        midiHandler.processOutgoing();
        LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
        LL_mDelay(timeSpan);
        msg = MidiMessage::makeNoteOff(midiChannel, 0x40, 100);
        midiHandler.enqueueOutgoing(msg);
        msg = MidiMessage::makeNoteOn(midiChannel, 0x41, 100);
        midiHandler.enqueueOutgoing(msg);
        midiHandler.processOutgoing();
        LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
        LL_mDelay(timeSpan);
        msg = MidiMessage::makeNoteOff(midiChannel, 0x41, 100);
        midiHandler.enqueueOutgoing(msg);
        msg = MidiMessage::makeNoteOn(midiChannel, 0x43, 100);
        midiHandler.enqueueOutgoing(msg);
        midiHandler.processOutgoing();
        LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
        LL_mDelay(timeSpan << 1);
        msg = MidiMessage::makeNoteOff(midiChannel, 0x43, 100);
        midiHandler.enqueueOutgoing(msg);
        msg = MidiMessage::makeNoteOn(midiChannel, 0x43, 100);
        midiHandler.enqueueOutgoing(msg);
        midiHandler.processOutgoing();
        LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
        LL_mDelay(timeSpan << 1);
        msg = MidiMessage::makeNoteOff(midiChannel, 0x43, 100);
        midiHandler.enqueueOutgoing(msg);
        midiHandler.processOutgoing();
        LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);

        //for(uint8_t i = 0; i < 2; ++i) {
        //    LL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin|LED_RED_Pin|LED_BLUE_Pin);
        //    uint32_t pinSet = LL_GPIO_IsOutputPinSet(GPIOB, LED_GREEN_Pin);
        //    //printf("loop %d\n", i);
        //    MidiMessage msg = pinSet ?
        //        MidiMessage::makeNoteOn(midiChannel, 0x3C, 100) :
        //        MidiMessage::makeNoteOff(midiChannel, 0x3C, 100);
        //    printf("Midi Send: ");
        //    MidiMessage::dump(msg);
        //    midiHandler.enqueueOutgoing(msg);
        //    midiHandler.processOutgoing();
        //    LL_mDelay(100);
        //}
    }
}

extern "C" {

void enqueueIncomingMidi(uint8_t *data) {
    MidiUSBMessage umsg(data);
    MidiMessage msg;
    umsg.getMidiMessage(msg);
    midiHandler.enqueueIncoming(msg);
}

}