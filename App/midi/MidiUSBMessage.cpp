#include "MidiUSBMessage.h"

MidiUSBMessage::MidiUSBMessage(uint8_t *data) {
    memcpy(_data, data, 4);
}

MidiUSBMessage::MidiUSBMessage(const uint8_t cableNumber, MidiMessage &message) {
    ChannelIndexNumber cin = getChannelIndexNumber(message.channelMessage());
    MidiUSBMessage(cableNumber, cin, message);
}

MidiUSBMessage::MidiUSBMessage(const uint8_t cableNumber, const uint8_t codeIndexNumber, MidiMessage &midiMessage) {
    setCableNumber(cableNumber);
    setCodeIndexNumber(codeIndexNumber);
    _data[1] = midiMessage.status();
    _data[2] = midiMessage.data0();
    _data[3] = midiMessage.data1();
}

ChannelIndexNumber MidiUSBMessage::getChannelIndexNumber(MidiMessage::ChannelMessage status) {
    ChannelIndexNumber ret;
    switch(status) {
    case MidiMessage::ChannelMessage::NoteOff:
        ret = NOTE_OFF;
        break;
    case MidiMessage::ChannelMessage::NoteOn:
        ret = NOTE_ON;
        break;
    case MidiMessage::ChannelMessage::KeyPressure:
        ret = POLYPHONIC_KEY_PRESSURE;
        break;
    case MidiMessage::ChannelMessage::ControlChange:
        ret = CONTROL_CHANGE;
        break;
    case MidiMessage::ChannelMessage::ProgramChange:
        ret = PROGRAM_CHANGE;
        break;
    case MidiMessage::ChannelMessage::ChannelPressure:
        ret = CHANNEL_PRESSURE;
        break;
    case MidiMessage::ChannelMessage::PitchBend:
        ret = PITCH_BEND;
        break;
    default:
        ret = INVALID;
        break;
    }
    return ret;
}