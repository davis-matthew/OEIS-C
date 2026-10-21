/*
 * A296885  Numbers whose base-11 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          = #(peaks); see Comments.
 * https://oeis.org/A296885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   eq_pits_peaks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth positive integer whose base‑11 digits
 * have the same number of pits and peaks. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/eq_pits_peaks.h"

/* Return the nth term of the sequence.  Uses a static cache so
 * that calls are efficient even if the indices are not monotonic. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t count = 0;
    static size_t cap   = 0;
    static seq_t last   = 0;

    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    while (count < n) {
        ++last;
        if (eq_pits_peaks(last)) {
            if (count >= cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[count++] = last;
        }
    }
    return cache[n - 1];
}
