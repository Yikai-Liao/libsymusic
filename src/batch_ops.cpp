//
// Created by lyk on 23-12-16.
//
#include "symusic/batch_ops.h"
#include "pdqsort.h"

namespace symusic::ops {

template<typename T, class Compare>
void sort_branchless(const vec<T> & data, Compare cmp) {
    pdqsort_branchless(data.begin(), data.end(), cmp);
}

template<trait::TimeEvent T>
void sort_branchless(const vec<T> & data) {
    pdqsort_branchless(data.begin(), data.end(), [](const T& a, const T& b) {
        return (a.time) < (b.time);
    });
}

template<class Iter, class Compare>
void sort_branchless(Iter begin, Iter end, Compare comp) {
    pdqsort_branchless(begin, end, comp);
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