/*
 * A294601  Numbers with exactly one odd decimal digit.
 * https://oeis.org/A294601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A294601: Numbers with exactly one odd decimal digit
 *
 *  a(n) = the nth positive integer that contains exactly one odd digit
 *         in its decimal expansion (decimal digits 0–9).
 *
 *  The algorithm enumerates integers in increasing order and keeps
 *  those that satisfy the property.  A small cache is maintained so
 *  that subsequent calls reuse previously computed terms.
 */

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

static inline bool has_exactly_one_odd_digit(seq_t x) {
    int odd_cnt = 0;
    if (x == 0) return false;          /* 0 has no odd digits */
    while (x > 0) {
        int d = (int)(x % 10);
        if (d & 1) {
            ++odd_cnt;
            if (odd_cnt > 1) return false;
        }
        x /= 10;
    }
    return odd_cnt == 1;
}

/*  External linkage as required  */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n=0; not used by callers */

#ifdef __cplusplus
    /*  C++ implementation using std::vector  */
    static std::vector<seq_t> cache;
    static seq_t next = 1;

    while (cache.size() < n) {
        /*  Find the next integer with exactly one odd digit  */
        while (!has_exactly_one_odd_digit(next)) ++next;
        cache.push_back(next);
        ++next;
    }
    return cache[n - 1];
#else
    /*  C implementation using dynamic array  */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t next = 1;

    while (len < n) {
        while (!has_exactly_one_odd_digit(next)) ++next;

        /*  Expand storage if needed  */
        if (len >= cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();               /* unexpected allocation failure */
            cache = tmp;
            cap = new_cap;
        }
        cache[len++] = next;
        ++next;
    }
    return cache[n - 1];
#endif
}
