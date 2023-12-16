//
// Created by lyk on 23-12-14.
//
#include "MiniMidi.hpp"
#include "symusic/utils.h"
#include "symusic/batch_ops.h"
#include "symusic/format/midi.h"
#include <queue>
#include <map>

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

// define some utils
template<typename T>
struct NoteOn: TimeStamp<T> {
    typedef typename T::unit unit;
    i8 velocity;
    NoteOn(): TimeStamp<T>(0), velocity(0) {}
    NoteOn(const unit time, i8 const velocity) : TimeStamp<T>(time), velocity(velocity) {};
    [[nodiscard]] bool empty() const {return velocity == 0;}
    void reset() { this->time=0; velocity=0; }
};

template<typename T>
struct NoteOnQueue {
private:
    typedef typename T::unit unit;
    std::queue<NoteOn<T>> queue;
    NoteOn<T> _front;

public:
    NoteOnQueue() = default;
    [[nodiscard]] size_t size() const { return _front.empty() ? 0 : queue.size() + 1; }

    [[nodiscard]] bool empty() const { return _front.empty();}

    [[nodiscard]] const NoteOn<T> &front() const { return _front; }

    void emplace(unit time, i8 velocity) {
        if(_front.empty()) _front = NoteOn<T>{time, velocity};
        else queue.emplace(time, velocity);
    }

    void pop() {
        if (queue.empty()) _front.reset();
        else {
            _front = queue.front();
            queue.pop();
        }
    }
};

typedef std::tuple<u8, u8> TrackIdx;

template <typename T>
Track<T> &get_track(
    std::map<TrackIdx, Track<T>> &track_map,
    Track<T> (& stragglers)[16],
    const uint8_t channel, const uint8_t program,
    const size_t message_num, const bool create_new) {
    auto track_idx = std::make_tuple(channel, program);
    const auto & entry = track_map.find(track_idx);
    if (entry == track_map.end()) {
        auto &track = stragglers[channel];
        if (!create_new) return track;
        Track<T> new_track;
        if (!track.empty()) {
            new_track = Track(track); // copy from stragglers
        }
        new_track.program = program;
        new_track.is_drum = channel == 9;
        new_track.notes.reserve(message_num / 2 + 10);
        return track_map[track_idx] = new_track;
    }
    return entry->second;
}

inline Score<tag::Tick> parse_midi_tick(const std::span<const u8> buffer) {
    namespace message = minimidi::message;

    // remove this redundant copy in the future
    const vec<u8> buffer_vec(buffer.begin(), buffer.end());
    minimidi::file::MidiFile midi(buffer_vec);
    i32 tpq = midi.get_tick_per_quarter();
    Score<tag::Tick> score(tpq);
    for(const auto &midi_track: midi.tracks) {
        // (channel, program) -> Track
        std::map<TrackIdx, Track<tag::Tick>> track_map;
        // channel -> Track
        Track<tag::Tick> stragglers[16];
        // channel -> program
        uint8_t cur_instr[16] = {0};
        std::string cur_name;
        // (channel, pitch) -> (start, velocity)
        NoteOnQueue<tag::Tick> last_note_on[16][128] = {};
        // channel -> pedal_on
        i32 last_pedal_on[16] = {-1};
        // iter midi messages in the track
        size_t message_num = midi_track.message_num();
        for(const auto &msg: midi_track.messages) {
            auto cur_tick = static_cast<i32>(msg.get_time());

            switch (msg.get_type()) {
                case (message::MessageType::NoteOn): {
                    if (uint8_t velocity = msg.get_velocity(); velocity != 0) {
                        auto pitch = msg.get_pitch();
                        if (pitch >= 128) throw std::range_error(
                            "Get pitch=" + std::to_string(static_cast<int>(pitch)));
                        if (velocity >= 128) throw std::range_error(
                            "Get velocity=" + std::to_string(static_cast<int>(velocity)));
                        last_note_on[msg.get_channel()][pitch].emplace(
                            cur_tick, static_cast<i8>(velocity));
                        break;
                    } // if velocity is zero, turn to note off case
                }
                case (message::MessageType::NoteOff): {
                    uint8_t channel = msg.get_channel();
                    uint8_t pitch = msg.get_pitch();
                    if (pitch >= 128) throw std::range_error(
                        "Get pitch=" + std::to_string((int) pitch));
                    auto &note_on_queue = last_note_on[channel][pitch];
                    auto &track = get_track(
                        track_map, stragglers, channel,
                        cur_instr[channel], message_num,true);
                    if ((!note_on_queue.empty()) && (cur_tick > note_on_queue.front().time)) {
                        auto const &note_on = note_on_queue.front();
                        uint32_t duration = cur_tick - note_on.time;
                        track.notes.emplace_back(
                            note_on.time, duration,
                            pitch, note_on.velocity
                        );
                        note_on_queue.pop();
                    }
                    break;
                }
                case (message::MessageType::ProgramChange): {
                    uint8_t channel = msg.get_channel();
                    uint8_t program = msg.get_program();
                    if (program >= 128) throw std::range_error(
                        "Get program=" + std::to_string((int) program));
                    cur_instr[channel] = program;
                    break;
                }
                case (message::MessageType::ControlChange): {
                    uint8_t channel = msg.get_channel();
                    uint8_t program = cur_instr[channel];
                    auto &track = get_track(
                        track_map, stragglers, channel, program, message_num, false);
                    uint8_t control_number = msg.get_control_number();
                    uint8_t control_value = msg.get_control_value();
                    if (control_number >= 128) throw std::range_error(
                        "Get control_number=" + std::to_string((int) control_number));
                    if (control_value >= 128) throw std::range_error(
                        "Get control_value=" + std::to_string((int) control_value));
                    track.controls.emplace_back(
                        cur_tick, msg.get_control_number(), msg.get_control_value());
                    // Pedal Part
                    if (control_number == 64) {
                        if (control_value >= 64) {
                            if (last_pedal_on[channel] < 0) last_pedal_on[channel] = cur_tick;
                        } else {
                            if (last_pedal_on[channel] >= 0) {
                                track.pedals.emplace_back(
                                    last_pedal_on[channel], cur_tick - last_pedal_on[channel]
                                );
                                last_pedal_on[channel] = -1;
                            }
                        }
                    }
                    break;
                }
                case (message::MessageType::PitchBend): {
                    uint8_t channel = msg.get_channel();
                    uint8_t program = cur_instr[channel];
                    auto &track = get_track(
                        track_map, stragglers, channel, program, message_num, false);
                    auto value = msg.get_pitch_bend();
                    if (value < message::MIN_PITCHBEND || value > message::MAX_PITCHBEND)
                        throw std::range_error("Get pitch_bend=" + std::to_string(value));
                    track.pitch_bends.emplace_back(
                        cur_tick, value
                    );
                    break;
                }
                    // Meta Message
                case (message::MessageType::Meta): {
                    switch (msg.get_meta_type()) {
                        case (message::MetaType::TrackName): {
                            auto data = msg.get_meta_value();
                            auto tmp = std::string(data.begin(), data.end());
                            cur_name = strip_non_utf_8(tmp);
                            break;
                        }
                        case (message::MetaType::TimeSignature): {
                            const auto [numerator, denominator] = msg.get_time_signature();
                            score.time_signatures.emplace_back(cur_tick, numerator, denominator);
                            break;
                        }
                        case (message::MetaType::SetTempo): {
                            score.tempos.emplace_back(cur_tick, 60000000.f / static_cast<float>(msg.get_tempo()));
                            break;
                        }
                        case (message::MetaType::KeySignature): {
                            const auto k_msg = msg.get_key_signature();
                            score.key_signatures.emplace_back(cur_tick, k_msg.key, k_msg.tonality);
                            break;
                        }
                        case (message::MetaType::Lyric): {
                            auto data = msg.get_meta_value();
                            auto tmp = std::string(data.begin(), data.end());
                            auto text = strip_non_utf_8(tmp);
                            if (text.empty()) break;
                            score.lyrics.emplace_back(cur_tick, text);
                            break;
                        }
                        case (message::MetaType::Marker): {
                            auto data = msg.get_meta_value();
                            auto tmp = std::string(data.begin(), data.end());
                            auto text = strip_non_utf_8(tmp);
                            if (text.empty()) break;
                            score.markers.emplace_back(cur_tick, text);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
        }
        for (auto [_, track]: std::move(track_map)) {
            if (track.empty()) continue;
            track.name = cur_name;
            track.notes.shrink_to_fit();
            score.tracks.push_back(std::move(track));
        }
    }
    ops::sort_branchless(score.time_signatures);
    ops::sort_branchless(score.key_signatures);
    ops::sort_branchless(score.tempos);
    ops::sort_branchless(score.lyrics);
    ops::sort_branchless(score.markers);
    return score;
} // parse_midi_tick

inline vec<u8> dumps_midi_tick(const Score<tag::Tick>& score) {
    namespace message = minimidi::message;

    minimidi::file::MidiFile midi{
        minimidi::file::MidiFormat::MultiTrack, 0,
        static_cast<u16>(score.ticks_per_quarter)
    };
    midi.tracks.reserve(score.tracks.size() + 1);
    {   // add meta messages
        message::Messages msgs{};
        msgs.reserve(
            score.time_signatures.size() + score.key_signatures.size() +
            score.tempos.size() + score.lyrics.size() + score.markers.size() + 10);
        // add time signatures
        for(const auto &time_signature: score.time_signatures) {
            msgs.emplace_back(message::Message::TimeSignature(
                time_signature.time, time_signature.numerator, time_signature.denominator
            ));
        }
        // add key signatures
        for(const auto &key_signature: score.key_signatures) {
            msgs.emplace_back(message::Message::KeySignature(
                key_signature.time, key_signature.key, key_signature.tonality
            ));
        }
        // add tempos
        for(const auto &tempo: score.tempos) {
            msgs.emplace_back(message::Message::SetTempo(
                tempo.time, static_cast<u32>(60000000.f / tempo.qpm)
            ));
        }
        // add lyrics
        for(const auto &lyric: score.lyrics) {
            msgs.emplace_back(message::Message::Lyric(lyric.time, lyric.text));
        }
        // add markers
        for(const auto &marker: score.markers) {
            msgs.emplace_back(message::Message::Marker(marker.time, marker.text));
        }
        ops::sort_branchless(msgs.begin(), msgs.end(), [](const auto &a, const auto &b) {
            return a.get_time() < b.get_time();
        });
        midi.tracks.emplace_back(std::move(msgs));
    }

    for(const auto &track: score.tracks) {
        message::Messages msgs{};
        msgs.reserve(track.note_num() * 2 + track.controls.size() + track.pitch_bends.size() + 10);
        const u8 channel = track.is_drum ? 9 : 0;
        // add track name
        if(!track.name.empty())
            msgs.emplace_back(message::Message::TrackName(0, track.name));
        // add program change
        msgs.emplace_back(message::Message::ProgramChange(0, channel, track.program));
        // Todo add check for Pedal
        // add control change
        for(const auto &control: track.controls) {
            msgs.emplace_back(message::Message::ControlChange(
                control.time, channel, control.number, control.value
            ));
        }
        // add pitch bend
        for(const auto &pitch_bend: track.pitch_bends) {
            msgs.emplace_back(message::Message::PitchBend(
                pitch_bend.time, channel, static_cast<i16>(pitch_bend.value)
            ));
        }
        // add notes
        for(const auto &note: track.notes) {
            msgs.emplace_back(message::Message::NoteOn(
                note.time, channel, note.pitch, note.velocity
            ));
            msgs.emplace_back(message::Message::NoteOff(
                note.end(), channel, note.pitch, note.velocity
            ));
        }
        ops::sort_branchless(msgs.begin(), msgs.end(), [](const auto &a, const auto &b) {
            return a.get_time() < b.get_time();
        });
        midi.tracks.emplace_back(std::move(msgs));
    }
    return midi.to_bytes();
} // dumps_midi_tick

}
