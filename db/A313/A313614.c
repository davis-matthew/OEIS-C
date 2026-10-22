/*
 * A313614  Coordination sequence Gal.5.129.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10 Initial terms:
 *            a(0)=1, a(1)=5, a(2)=10, a(3)=14, a(4)=20, a(5)=24, a(6)=30,
 *            a(7)=34, a(8)=39, a(9)=44, a(10)=49
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialise base terms once */
    static const seq_t base[11] = {
        (seq_t)1, (seq_t)5, (seq_t)10, (seq_t)14, (seq_t)20,
        (seq_t)24, (seq_t)30, (seq_t)34, (seq_t)39, (seq_t)44, (seq_t)49
    };

    /* create the cache if it does not exist yet */
    if (cache == NULL) {
        cache_size = 11;                          /* holds a(0)..a(10) */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) {                       /* allocation failed */
            /* if allocation fails return 0 as a safe fallback */
            return (seq_t)0;
        }
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = base[i];
    }

    /* expand cache to include a(n) if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (new_cache == NULL) {                  /* allocation failed */
            return (seq_t)0;
        }
        cache = new_cache;

        for (size_t i = cache_size; i < new_size; ++i) {
            /* recurrence: a(i) = a(i-1) + a(i-9) - a(i-10) */
            cache[i] = cache[i-1] + cache[i-9] - cache[i-10];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
