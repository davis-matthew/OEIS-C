/*
 * A314220  Coordination sequence Gal.5.306.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10) Initial
 *            values: a(0)=1, a(1)=5, a(2)=11, a(3)=17, a(4)=22, a(5)=28,
 *            a(6)=34, a(7)=39, a(8)=45, a(9)=51, a(10)=56
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system */
static const seq_t init[11] = {
    (seq_t)1, (seq_t)5, (seq_t)11, (seq_t)17, (seq_t)22,
    (seq_t)28, (seq_t)34, (seq_t)39, (seq_t)45, (seq_t)51, (seq_t)56
};

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* first call: initialize cache with known values */
    if (cache_size == 0) {
        cache_size = 11;                      /* indices 0..10 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init[i];
    }

    /* present values are valid for n <= 10 */
    if (n < cache_size)
        return cache[n];

    /* enlarge cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return 0 as a graceful failure */
            free(cache);
            cache = NULL;
            cache_size = 0;
            return (seq_t)0;
        }
        cache = tmp;
        /* compute new entries sequentially */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* use signed arithmetic to avoid unsigned underflow */
            long long val =
                2LL * (long long)cache[i-1]
              - 2LL * (long long)cache[i-2]
              + 2LL * (long long)cache[i-3]
              - 2LL * (long long)cache[i-4]
              + 2LL * (long long)cache[i-5]
              - 2LL * (long long)cache[i-6]
              + 2LL * (long long)cache[i-7]
              - 2LL * (long long)cache[i-8]
              + 2LL * (long long)cache[i-9]
              -   (long long)cache[i-10];
            cache[i] = (seq_t)val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
