#pragma once

#include "utils/RingBuffer.h"
#include "midi/MidiMessage.h"
#include "midi/MidiUSBMessage.h"
#include "usbd_midi_if.h"

#define MAX_ATTEMPTS      10

extern USBD_MIDI_ItfTypeDef USBD_MIDI_fops_FS;

class MidiHandler {
public:
    MidiHandler() = default;
    ~MidiHandler() = default;

    inline void init() {
        _incoming.init();
        _outgoing.init();
    }

    inline void enqueueOutgoing(MidiMessage &msg) {
        _outgoing.write(msg);
    }

    inline bool outgoingIsFull() {
        return _outgoing.full();
    }

    inline bool outgoingIsEmpty() {
        return _outgoing.empty();
    }

    inline bool dequeueOutgoing(MidiMessage* msg) {
        if(outgoingIsEmpty()) {
            return false;
        }
        _outgoing.read(msg, 1);
        return true;
    }

    inline void enqueueIncoming(MidiMessage &msg) {
        _incoming.write(msg);
    }

    inline bool incomingIsFull() {
        return _incoming.full();
    }

    inline bool incomingIsEmpty() {
        return _incoming.empty();
    }

    inline bool dequeueIncoming(MidiMessage* msg) {
        if(incomingIsEmpty()) {
            return false;
        }
        _incoming.read(msg, 1);
        return true;
    }

    inline void processOutgoing() {
        MidiMessage msg;
        uint8_t ret; // USBD_StatusTypeDef
        while(dequeueOutgoing(&msg)) {
            MidiUSBMessage umsg(0, msg);
            //MidiUSBMessage::dump(umsg);
            uint8_t i = 0;
            do {
                ret = USBD_MIDI_fops_FS.Send(umsg.getData(), 4);

                //LL_mDelay(500);
                //printf("USB Send #%d: status = %d\n", i, ret);
            } while(ret != USBD_OK && i++ < MAX_ATTEMPTS);
        }
    }

private:
    RingBuffer<MidiMessage, 20> _incoming;
    RingBuffer<MidiMessage, 20> _outgoing;
};
