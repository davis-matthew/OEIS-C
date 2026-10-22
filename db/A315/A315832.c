/*
 * A315832  Coordination sequence Gal.5.207.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-5) - a(n-10) a(0)=1, a(1)=6, a(2)=7, a(3)=11,
 *            a(4)=14, a(5)=20, a(6)=26, a(7)=25, a(8)=29, a(9)=34, a(10)=40
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache to store computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static int initialized = 0;

    /* initial terms up to n=10 */
    static const seq_t init_vals[11] = {
        (seq_t)1, (seq_t)6, (seq_t)7, (seq_t)11, (seq_t)14,
        (seq_t)20, (seq_t)26, (seq_t)25, (seq_t)29, (seq_t)34, (seq_t)40
    };

    /* Initialize cache on first call */
    if (!initialized) {
        cache_size = 11;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = init_vals[i];
        initialized = 1;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = cache_size;
        while (new_size <= (size_t)n)
            new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0; /* allocation failed, return 0 */
        cache = tmp;
        /* fill new slots with 0 temporarily */
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* Compute missing values up to n */
    if ((size_t)n >= 11) {
        for (size_t i = 11; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)2 * cache[i - 5] - cache[i - 10];
        }
    }

    return cache[(size_t)n];
}
