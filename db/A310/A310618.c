/*
 * A310618  Coordination sequence Gal.4.121.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-5) - a(n-10) for n > 11 Initial values: a(0)=1,
 *            a(1)=4, a(2)=6, a(3)=14, a(4)=16, a(5)=18, a(6)=28, a(7)=26,
 *            a(8)=36, a(9)=36, a(10)=38, a(11)=50 The function memoizes the
 *            sequence in a static array and grows the array on demand in O(1)
 *            amortised each call
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values.
       cache[0..cache_size-1] are valid. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Init with base values if necessary. */
    if (cache == NULL) {
        cache_size = 12;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) exit(1);   /* allocation failed */

        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)4;
        cache[2]  = (seq_t)6;
        cache[3]  = (seq_t)14;
        cache[4]  = (seq_t)16;
        cache[5]  = (seq_t)18;
        cache[6]  = (seq_t)28;
        cache[7]  = (seq_t)26;
        cache[8]  = (seq_t)36;
        cache[9]  = (seq_t)36;
        cache[10] = (seq_t)38;
        cache[11] = (seq_t)50;
    }

    /* Return cached value if already available. */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to hold indices up to n. */
    size_t new_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) exit(1);  /* allocation failed */
    cache = tmp;

    /* Compute values from old size up to n (inclusive). */
    for (size_t i = cache_size; i <= n; ++i) {
        /* a(i) = 2*a(i-5) - a(i-10) for i >= 12. */
        cache[i] = (seq_t)( (seq_t)2 * cache[i - 5] - cache[i - 10] );
    }

    cache_size = new_size;
    return cache[n];
}
