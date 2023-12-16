//
// Created by lyk on 23-12-16.
//
#include "symusic/batch_ops.h"
#include "pdqsort.h"

namespace symusic::ops {

template<trait::TimeEvent T>
void sort_branchless(const vec<T> & events, std::function<bool(const T&, const T&)> cmp) {
    pdqsort_branchless(events.begin(), events.end(), cmp);
}

template<class T, class Compare>
void sort(vec<T>& data, Compare cmp) {
    pdqsort(data.begin(), data.end(), cmp);
}

template<class Iter, class Compare>
void sort(Iter begin, Iter end, Compare comp) {
    pdqsort(begin, end, comp);
}
}