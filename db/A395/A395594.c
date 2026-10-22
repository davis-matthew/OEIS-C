/*
 * A395594  a(n) = a(n-1) + a(n-3) + a(n-4), a(0)=1, a(1)=9, a(2)=81, a(3)=90.
 * https://oeis.org/A395594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A395594:
   a(0)=1, a(1)=9, a(2)=81, a(3)=90,
   a(n)=a(n-1)+a(n-3)+a(n-4) for n≥4
   (Recurrence form with explicit initial values.) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache         = NULL;
    static size_t cache_cap     = 0;   /* capacity of cache array  */
    static idx_t  cache_len     = 0;   /* number of valid terms 0..len-1 */

    /* ensure enough space for indices up to n */
    size_t needed = (size_t)(n + 1);

    if (cache == NULL) {
        /* first call: allocate at least space for the 4 initial terms */
        cache_cap = (needed > 4) ? needed : 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)81;
        cache[3] = (seq_t)90;
        cache_len = 4;
    } else if (needed > cache_cap) {
        /* grow the cache when necessary */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = needed;
    }

    /* compute all missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i >= 4) {
            cache[(size_t)i] = cache[(size_t)(i - 1)]
                             +  cache[(size_t)(i - 3)]
                             +  cache[(size_t)(i - 4)];
        }
        /* i<4 terms are already initialized */
    }
    cache_len = n + 1;

    return cache[(size_t)n];
}
