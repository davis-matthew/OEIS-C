/*
 * A296895  Numbers whose base-14 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          > #(peaks); see Comments.
 * https://oeis.org/A296895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9765 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A296895: numbers in base‑14 whose number of pits exceeds the number of peaks. */
/* Searches sequentially and memoises found values. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Helper: true if base‑14 representation of v has more pits than peaks. */
static bool more_pits_than_peaks(seq_t v)
{
    /* Extract base‑14 digits (least significant first). */
    int digits[64];
    int len = 0;
    seq_t x = v;
    if (x == (seq_t)0) {
        /* Zero has no pits or peaks. */
        return false;
    }
    while (x > (seq_t)0) {
        digits[len++] = (int)(x % (seq_t)14);
        x /= (seq_t)14;
    }

    /* Count pits and peaks, ignoring boundary digits as they can't be local extrema. */
    int pits = 0, peaks = 0;
    for (int i = 1; i <= len - 2; ++i) {
        int left  = digits[i - 1];
        int cur   = digits[i];
        int right = digits[i + 1];
        if (left > cur && cur < right) ++pits;
        else if (left < cur && cur > right) ++peaks;
    }
    return pits > peaks;
}

seq_t a(idx_t n)
{
    /* Static cache of found values. */
    static seq_t  *cache   = NULL;
    static size_t  cache_size = 0;
    static size_t  cache_cap  = 0;
    static seq_t   last_checked = (seq_t)0;

    /* Expand cache if necessary and compute more values until we have n terms. */
    while (cache_size < n) {
        last_checked = last_checked + (seq_t)1;
        if (more_pits_than_peaks(last_checked)) {
            /* Ensure capacity. */
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = last_checked;
        }
    }
    return cache[n - 1];
}
