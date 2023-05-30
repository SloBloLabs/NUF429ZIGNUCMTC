#pragma once

#include "MidiMessage.h"

// CIN
//#define MIDI_CIN_SYSCOMMON_2BYTES          0x02
//#define MIDI_CIN_SYSCOMMON_3BYTES          0x03
//#define MIDI_CIN_SYSEX_START_OR_CONTINUE   0x04
//#define MIDI_CIN_SYSEX_END_1BYTE           0x05
//#define MIDI_CIN_SYSEX_END_2BYTES          0x06
//#define MIDI_CIN_SYSEX_END_3BYTES          0x07
//#define MIDI_CIN_NOTE_OFF                  0x08
//#define MIDI_CIN_NOTE_ON                   0x09
//#define MIDI_CIN_POLYPHONIC_KEY_PRESSURE   0x0A
//#define MIDI_CIN_CONTROL_CHANGE            0x0B
//#define MIDI_CIN_PROGRAM_CHANGE            0x0C
//#define MIDI_CIN_CHANNEL_PRESSURE          0x0D
//#define MIDI_CIN_PITCH_BEND                0x0E
//#define MIDI_CIN_SYS_REALTIME              0x0F

enum ChannelIndexNumber {
    INVALID                 = 0x00,
    SYSCOMMON_2BYTES        = 0x02,
    SYSCOMMON_3BYTES        = 0x03,
    SYSEX_START_OR_CONTINUE = 0x04,
    SYSEX_END_1BYTE         = 0x05,
    SYSEX_END_2BYTES        = 0x06,
    SYSEX_END_3BYTES        = 0x07,
    NOTE_OFF                = 0x08,
    NOTE_ON                 = 0x09,
    POLYPHONIC_KEY_PRESSURE = 0x0A,
    CONTROL_CHANGE          = 0x0B,
    PROGRAM_CHANGE          = 0x0C,
    CHANNEL_PRESSURE        = 0x0D,
    PITCH_BEND              = 0x0E,
    SYS_REALTIME            = 0x0F
};

// SYS EX
//#define MIDI_SYSEX_BEGIN                   0xF0
//#define MIDI_SYSEX_END                     0xF7

// Manufacturer ID
//#define MIDI_MANID_NON_COMMMERCIAL         0x7D
//#define MIDI_MANID_NON_REALTIME            0x7E
//#define MIDI_MANID_REALTIME                0x7F

// Channel Voice Message
//#define MIDI_NOTE_OFF                      0x80
//#define MIDI_NOTE_ON                       0x90
//#define MIDI_POLYPHONIC_KEY_PRESSURE       0xA0
//#define MIDI_CONTROL_CHANGE                0xB0
//#define MIDI_PROGRAM_CHANGE                0xC0
//#define MIDI_CHANNEL_PRESSURE              0xD0
//#define MIDI_PITCH_BEND                    0xE0

// Channel Mode Message
//#define MIDI_ALL_SOUND_OFF                 0x78
//#define MIDI_RESET_ALL_CONTROLLER          0x79
//#define MIDI_LOCAL_CONTROL                 0x7A
//#define MIDI_ALL_NOTE_OFF                  0x7B

// Midi Mode Config
//#define MIDI_OMNI_ON                       0x7D
//#define MIDI_OMNI_OFF                      0x7C
//#define MIDI_POLY                          0x7F
//#define MIDI_MONO                          0x7E

// System Realtime
//#define MIDI_TIMING_CLOCK                  0xF8
//#define MIDI_START                         0xFA
//#define MIDI_CONTINUE                      0xF8
//#define MIDI_STOP                          0xFC
//#define MIDI_ACTIVE_SENSING                0xFE
//#define MIDI_SYSTEM_RESET                  0xFF

class MidiUSBMessage {
public:
    MidiUSBMessage(uint8_t *data);
    MidiUSBMessage(const uint8_t cableNumber, MidiMessage &midiMessage);
    MidiUSBMessage(const uint8_t cableNumber, const uint8_t codeIndexNumber, MidiMessage &midiMessage);
    ~MidiUSBMessage() = default;

    inline uint8_t cableNumber() const {
        return (_data[0] >> 4) & 0x0F;
    }

    inline void setCableNumber(const uint8_t cableNumber) {
        _data[0] = (cableNumber << 4) | (_data[0] & 0x0F);
    }

    inline uint8_t codeIndexNumber() const {
        return _data[0] & 0x0F;
    }

    inline void setCodeIndexNumber(const uint8_t cin) {
        _data[0] = (cin & 0x0F) | (_data[0] & 0xF0);
    }

    inline void getMidiMessage(MidiMessage &midiMessage) const {
        midiMessage = MidiMessage(&_data[1]);
    }

    inline uint8_t midiChannel() const {
        return _data[1] & 0x0F;
    }

    inline uint8_t* getData() {
        return _data;
    }

private:
    static ChannelIndexNumber getChannelIndexNumber(MidiMessage::ChannelMessage status);

    uint8_t _data[4];
};