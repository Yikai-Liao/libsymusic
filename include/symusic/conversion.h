//
// Created by lyk on 23-12-17.
//

#ifndef LIBSYMUSIC_CONVERSION_H
#define LIBSYMUSIC_CONVERSION_H

#include "symusic/time_unit.h"
#include "symusic/track.h"
#include "symusic/score.h"
#include "symusic/note_arr.h"

namespace symusic {

template< TType To, TType From>
Score<To> convert(const Score<From> & score);

template<TType From>
Score<Tick> convert(const Score<From> & score, Tick::unit min_dir);

template<TType T>
NoteArr<T> to_note_arr(const Track<T> & track);

template<TType T>
Track<T> to_track(const NoteArr<T> & note_arr);

}
#endif //LIBSYMUSIC_CONVERSION_H
