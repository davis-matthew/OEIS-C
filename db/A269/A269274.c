/*
 * A269274  Number of nX6 0..3 arrays with some element plus some horizontally
 *          or antidiagonally adjacent neighbor totalling three exactly once.
 * https://oeis.org/A269274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 564*a(n-1) -87462*a(n-2) +2257724*a(n-3) *
 *            -21169617*a(n-4) +76236552*a(n-5) -92236816*a(n-6), n>7 *
 *            Initial terms * a(1)=1620 * a(2)=1104264 * a(3)=474819408 *
 *            a(4)=174924572760 * a(5)=59587625651904 * a(6)=19356924219624936
 *            * a(7)=6090616046325570480
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Initial values (0‑based indices 0..6 correspond to a(1)..a(7)) */
    static const seq_t init[7] = {
        (seq_t)1620,
        (seq_t)1104264,
        (seq_t)474819408,
        (seq_t)174924572760ULL,
        (seq_t)59587625651904ULL,
        (seq_t)19356924219624936ULL,
        (seq_t)6090616046325570480ULL
    };

    /* Cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache == NULL) {
        /* Allocate space for at least the initial 7 terms */
        cache_size = 7;
        cache = (seq_t *)realloc(NULL, cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init[i];
    }

    if (n > cache_size) {
        /* Grow the cache to size n */
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;          /* allocation failed */
        cache = new_cache;

        /* Compute any new terms using the recurrence */
        for (idx_t i = cache_size; i < n; ++i) {
            if (i < 6) {
                /* i is 0..5, already initialized */
                continue;
            }
            seq_t v = cache[i-1] * (seq_t)564;
            v -= cache[i-2] * (seq_t)87462;
            v += cache[i-3] * (seq_t)2257724;
            v -= cache[i-4] * (seq_t)21169617;
            v += cache[i-5] * (seq_t)76236552;
            v -= cache[i-6] * (seq_t)92236816;
            cache[i] = v;
        }
        cache_size = n;
    }

    return cache[n-1];
}
