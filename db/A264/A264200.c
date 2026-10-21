/*
 * A264200  Numerator of sum of numbers in set g(n) generated as in Comments.
 * https://oeis.org/A264200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Conjecture: a(n) = 3*a(n-1) + 4*a(n-2) – 8*a(n-3) – 8*a(n-4);
   Initial values: a(0)=0, a(1)=1, a(2)=5, a(3)=19 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for previously computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0; /* current capacity in elements   */
    static size_t cache_len = 0; /* number of elements stored      */

    /* Initialise cache on first use */
    if (!cache)
    {
        cache_cap = 4;                          /* at least the first four terms */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;                           /* allocation failure */

        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 5;
        cache[3] = 19;
        cache_len = 4;                          /* next index to compute      */
    }

    /* Return cached value if already known */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Grow the cache so that index n is valid */
    if ((size_t)n >= cache_cap)
    {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                       /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t val;
        /* a(i) = 3*a(i-1) + 4*a(i-2) – 8*a(i-3) – 8*a(i-4)  for i ≥ 4 */
        val  = cache[i-1] * (seq_t)3 + cache[i-2] * (seq_t)4;
        val -= cache[i-3] * (seq_t)8;
        val -= cache[i-4] * (seq_t)8;
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
