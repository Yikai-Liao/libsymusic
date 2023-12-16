//
// Created by lyk on 23-12-14.
//
#include "MiniMidi.hpp"
#include "symusic/format/midi.h"

namespace symusic::details {
// The parsing midi in quarter and second is dependent on the tick
inline Score<tag::Quarter> parse_midi_quarter(const std::span<u8> buffer) {
    return parse_midi_tick(buffer).to<tag::Quarter>();
}

inline Score<tag::Second> parse_midi_second(const std::span<u8> buffer) {
    return parse_midi_tick(buffer).to<tag::Second>();
}

vec<u8> dumps_midi_quarter(const Score<tag::Quarter>& score) {
    return score.to<tag::Tick>().dumps<tag::Midi>();
}

vec<u8> dumps_midi_second(const Score<tag::Second>& score) {
    return score.to<tag::Tick>().dumps<tag::Midi>();
}

inline Score<tag::Tick> parse_midi_tick(const std::span<u8> buffer) {
    throw std::runtime_error("Not implemented");
}

inline vec<u8> dumps_midi_tick(const Score<tag::Tick>& score) {
    throw std::runtime_error("Not implemented");
}

}   // symusic