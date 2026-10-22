/*
 * A310398  Coordination sequence Gal.5.136.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(1)=4; a(2)=10; for n>=3:
   a(n)=a(n-1)+p[(n-3) mod 10] with p={4,5,5,5,5,4,6,4,4,6} */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t stored = 0;   /* number of terms stored in cache */

    /* initialise cache with a(0) */
    if (cap == 0) {
        cap = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        stored = 1;
    }

    if ((size_t)n < stored) {
        return cache[(size_t)n];
    }

    static const seq_t pattern[10] = {
        (seq_t)4, (seq_t)5, (seq_t)5, (seq_t)5,
        (seq_t)5, (seq_t)4, (seq_t)6, (seq_t)4,
        (seq_t)4, (seq_t)6
    };

    for (size_t i = stored; i <= (size_t)n; ++i) {
        seq_t diff;
        if (i == 1) {
            diff = (seq_t)3;
        } else if (i == 2) {
            diff = (seq_t)6;
        } else {
            diff = pattern[(i - 3) % 10];
        }

        if (stored >= cap) {
            cap *= 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[stored] = cache[stored - 1] + diff;
        ++stored;
    }

    return cache[(size_t)n];
}
