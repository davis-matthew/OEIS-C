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
 * Validated: standard, pass, 83 terms through n = 9533 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth positive integer whose base‑11 digits
 * have the same number of pits and peaks. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if the base‑11 representation of x has equal pit and peak counts. */
static bool eq_pits_peaks(seq_t x)
{
    unsigned d[64];          /* enough slots for 64‑bit numbers and more. */
    int len = 0;
    seq_t t = x;

    if (t == 0) {
        d[0] = 0;
        len = 1;
    } else {
        while (t > 0) {
            d[len++] = (unsigned)(t % 11);
            t /= 11;
        }
    }

    /* Numbers with fewer than 3 digits automatically satisfy the condition. */
    if (len < 3)
        return true;

    int pits = 0, peaks = 0;
    for (int i = 1; i + 1 < len; ++i) {
        unsigned left  = d[i - 1];
        unsigned mid   = d[i];
        unsigned right = d[i + 1];
        if (left > mid && mid < right)
            ++pits;
        else if (left < mid && mid > right)
            ++peaks;
    }
    return pits == peaks;
}

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
        cache = mem_new(seq_t, cap);
    }

    while (count < n) {
        ++last;
        if (eq_pits_peaks(last)) {
            if (count >= cap) {
                cap *= 2;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[count++] = last;
        }
    }
    return cache[n - 1];
}
