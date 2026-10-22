/*
 * A313216  Coordination sequence Gal.6.367.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313216
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

/* Sequence A313216 (Coordination sequence Gal.6.367.4)
   Recurrence (for n > 12):
      a(n) = 2*a(n-1) - 2*a(n-2) + a(n-3) + a(n-4)
             - 2*a(n-5) + 2*a(n-6) - a(n-7)
   Initial values:
      a(0)=1, a(1)=4, a(2)=9, a(3)=15, a(4)=19, a(5)=25,
      a(6)=30, a(7)=36, a(8)=38, a(9)=44, a(10)=50,
      a(11)=58, a(12)=60
   The function caches computed values so that calls in any order
   are answered in O(1) after the first computation for each index. */

#include <stdlib.h>   /* malloc, realloc, abort, size_t */

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;

    /* base values for indices 0..12 */
    static const seq_t base[13] = {
        (seq_t)1, (seq_t)4, (seq_t)9, (seq_t)15, (seq_t)19,
        (seq_t)25, (seq_t)30, (seq_t)36, (seq_t)38, (seq_t)44,
        (seq_t)50, (seq_t)58, (seq_t)60
    };

    size_t idx = (size_t)n;          /* index into the cache */

    /* initialise cache on first call */
    if (!cache)
    {
        cache_size = 13;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            abort();                /* allocation failure */
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = base[i];
    }

    /* if requested term already cached, return it */
    if (idx < cache_size)
        return cache[idx];

    /* grow cache to include the term at index idx */
    size_t new_size = idx + 1;
    if (new_size > cache_size)
    {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();                /* allocation failure */
        cache      = tmp;
        cache_size = new_size;
    }

    /* compute missing terms using the recurrence */
    for (size_t k = 13; k <= idx; ++k)
    {
        seq_t term =
            cache[k-1] * (seq_t)2          /* 2*a(k-1) */
          - cache[k-2] * (seq_t)2          /* -2*a(k-2) */
          + cache[k-3]                     /* + a(k-3) */
          + cache[k-4]                     /* + a(k-4) */
          - cache[k-5] * (seq_t)2          /* -2*a(k-5) */
          + cache[k-6] * (seq_t)2          /* +2*a(k-6) */
          - cache[k-7];                    /* - a(k-7) */
        cache[k] = term;
    }

    return cache[idx];
}
