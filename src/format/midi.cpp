//
// Created by lyk on 23-12-14.
//

#include "symusic/format/midi.h"

namespace symusic::details {

Score<tag::Tick> parse_midi_tick(const std::span<u8> buffer) {
    throw std::runtime_error("Not implemented");
}

Score<tag::Quarter> parse_midi_quarter(const std::span<u8> buffer) {
    return parse_midi_tick(buffer).to<tag::Quarter>();
}

Score<tag::Second> parse_midi_second(const std::span<u8> buffer) {
    return parse_midi_tick(buffer).to<tag::Second>();
}
}   // symusic