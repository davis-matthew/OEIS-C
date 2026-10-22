/*
 * A301711  Partial sums of A301710.
 * https://oeis.org/A301711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A301711 – partial sums of A301710
   Recurrence: a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 3*a(n-4) + a(n-5)
   with initial values: a(0)=1, a(1)=6, a(2)=17, a(3)=34, a(4)=56
   (Defined for all n ≥ 0). */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cached_up_to = 4;   /* last index with a known value */

    /* create cache on first call */
    if (cache_cap == 0) {
        cache_cap = 5;                /* indices 0..4 inclusive */
        cache = alloc_seq_array(cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)34;
        cache[4] = (seq_t)56;
    }

    /* index in size_t; n is idx_t (unsigned 64‑bit) */
    size_t i = (size_t)n;

    /* already computed? */
    if (i <= cached_up_to)
        return cache[i];

    /* enlarge cache if necessary */
    if (i + 1 > cache_cap)
        grow_seq_array(&cache, &cache_cap, i + 1);

    /* compute missing terms up to i */
    for (size_t j = cached_up_to + 1; j <= i; ++j) {
        seq_t t = (seq_t)3 * cache[j-1];
        t -= (seq_t)4 * cache[j-2];
        t += (seq_t)4 * cache[j-3];
        t -= (seq_t)3 * cache[j-4];
        t += cache[j-5];
        cache[j] = t;
    }

    cached_up_to = i;
    return cache[i];
}
