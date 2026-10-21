/*
 * A296899  Numbers whose base-15 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          < #(peaks); see Comments.
 * https://oeis.org/A296899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   computed by searching for numbers whose base‑15 digits have
 *            fewer pits than peaks. The first element (a(1)) is 255
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pits_lt_peaks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */
#include <stdbool.h>
#include "../utils/pits_lt_peaks.h"

/* Sequence a(n).  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Static cache of qualifying numbers. */
    static seq_t *cache = NULL;          /* array of seq_t */
    static size_t cap  = 0;              /* allocated capacity */
    static idx_t  count = 0;             /* number of elements stored */
    static seq_t next_start = 255;       /* next number to test (>=255) */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();            /* out of memory */
        count = 0;
        /* next_start already set to 255 in declaration */
    }

    /* Extend cache until it contains at least n elements. */
    while (count < n) {
        if (pits_lt_peaks(next_start)) {
            /* Grow cache if needed. */
            if ((size_t)(count + 1) > cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[count++] = next_start;
        }
        ++next_start;                    /* test next number */
    }

    return cache[n - 1];
}
