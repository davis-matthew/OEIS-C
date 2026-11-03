/*
 * A366208  Numbers whose median of the digits is equal to 2.
 * https://oeis.org/A366208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose median of decimal digits equals 2, found by brute‐force search.
 * OEIS A366208:  a(n) = the n‑th integer whose sorted median digit is 2.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_frequencies.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Return 1 if the decimal representation of x has median digit 2. */
static int has_median_two(seq_t x)
{
    unsigned int freq[10];
    digit_frequencies(x, freq);

    size_t len = 0;
    for (int d = 0; d < 10; d++) len += freq[d];

    if ((len & 1) != 0) {                      /* odd number of digits */
        size_t pos = len / 2;                 /* 0‑based index of the median */
        size_t cum = 0;
        int median = -1;
        for (int d = 0; d < 10; d++) {
            cum += freq[d];
            if (cum > pos) { median = d; break; }
        }
        return median == 2;
    } else {                                   /* even number of digits */
        size_t pos1 = len / 2 - 1;
        size_t pos2 = len / 2;
        size_t cum = 0;
        int d1 = -1, d2 = -1;
        for (int d = 0; d < 10; d++) {
            cum += freq[d];
            if (cum > pos1 && d1 == -1) d1 = d;
            if (cum > pos2 && d2 == -1) { d2 = d; break; }
        }
        return (d1 + d2) == 4;   /* (d1+d2)/2 == 2  <=>  d1+d2 == 4 */
    }
}

/* Return the n‑th term of the sequence.  Index n ≥ 1. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 1;   /* integer to test next */

    if (cache_cap == 0) {
        cache_cap = 16;
        cache = seq_alloc(cache_cap);
    }

    while (cache_len < n) {
        seq_t cur = (seq_t)next_candidate;
        if (has_median_two(cur)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap << 1;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
