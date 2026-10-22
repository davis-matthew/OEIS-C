/*
 * A315301  Coordination sequence Gal.6.584.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=10, a(3)=16, a(4)=19, a(5)=30, * a(6)=31,
 *            a(7)=36, a(8)=42, a(9)=44, a(10)=60, * a(n) = 2*a(n-5) - a(n-10)
 *            for n > 10
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* pointer to cached values       */
    static idx_t cache_size = 0;         /* number of elements currently in cache */

    /* initially fill the cache with the first 11 known terms       */
    if (cache_size == 0) {
        cache_size = 11;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 6;
        cache[2] = 10;
        cache[3] = 16;
        cache[4] = 19;
        cache[5] = 30;
        cache[6] = 31;
        cache[7] = 36;
        cache[8] = 42;
        cache[9] = 44;
        cache[10] = 60;
    }

    /* grow the cache if the requested index is beyond its limits   */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {  /* allocation failed – abort with some error value */
            return 0;
        }
        cache = tmp;
        /* compute missing values using the recurrence                */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i <= 10) {
                /* the first 11 terms are already initialized */ ;
            } else {
                cache[i] = cache[i-5] * 2 - cache[i-10];
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
