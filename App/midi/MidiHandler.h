#pragma once

#include "utils/RingBuffer.h"
#include "midi/MidiMessage.h"
#include "midi/MidiUSBMessage.h"
#include "usbd_midi_if.h"

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

    inline void dequeueOutgoing(MidiMessage* msg) {
        _outgoing.read(msg, 1);
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

    inline void dequeueIncoming(MidiMessage* msg) {
        _incoming.read(msg, 1);
    }

    inline void processOutgoing() {
        MidiMessage msg;
        uint8_t ret; // USBD_StatusTypeDef
        while(!outgoingIsEmpty()) {
            dequeueOutgoing(&msg);
            MidiMessage::dump(msg);
            MidiUSBMessage umsg(0, msg);
            do {
                ret = MIDI_sendMessage(umsg.getData(), 4);
            } while(ret != USBD_OK);
        }
    }

private:
    RingBuffer<MidiMessage, 20> _incoming;
    RingBuffer<MidiMessage, 20> _outgoing;
};
