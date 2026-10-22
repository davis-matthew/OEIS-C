/*
 * A310744  Coordination sequence Gal.4.11.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310744
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

/* Recurrence: a(n) = a(n-3) + a(n-6) - a(n-9)  for n > 9
   Initial values: a(0)=1, a(1)=4, a(2)=7, a(3)=11, a(4)=16,
                   a(5)=18, a(6)=23, a(7)=27, a(8)=29, a(9)=34 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoized terms */
    static size_t cache_cap = 0;     /* allocated capacity */

    size_t idx = (size_t)n;
    if (idx + 1 > cache_cap) {
        /* need to extend cache */
        size_t need = idx + 1;
        seq_t *new_cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!new_cache)
            exit(1); /* allocation failure – not expected for tested limits */

        /* copy old terms if any */
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }

        /* initial known terms (0 … 9) */
        static const seq_t init[10] = {
            (seq_t)1, (seq_t)4, (seq_t)7, (seq_t)11,
            (seq_t)16, (seq_t)18, (seq_t)23, (seq_t)27,
            (seq_t)29, (seq_t)34
        };

        /* fill new entries */
        size_t start = cache_cap;   /* first index to compute */
        for (size_t i = start; i < need; ++i) {
            if (i < 10) {
                new_cache[i] = init[i];
            } else {
                new_cache[i] = new_cache[i-3] + new_cache[i-6] - new_cache[i-9];
            }
        }
        cache = new_cache;
        cache_cap = need;
    }

    return cache[idx];
}
