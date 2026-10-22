/*
 * A301288  Partial sums of A301287.
 * https://oeis.org/A301288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 221, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-1) - 2*a(n-2) + 3*a(n-3) - 3*a(n-4) + 
 *              2*a(n-5) - 2*a(n-6) + a(n-7)
 * Initial terms: a(0)=1, a(1)=4, a(2)=10, a(3)=17, a(4)=25, 
 *                a(5)=40, a(6)=58, a(7)=75, a(8)=95
 *
 * This implementation memoises all computed terms.  Every arithmetic
 * operation is performed in type seq_t by first casting the integer
 * constants to seq_t.  The function is thread‑safe only with respect
 * to a single shared cache; it does not use any extra synchronization.
 */

#include <stdlib.h>    /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;   /* number of elements currently allocated */

    /* initialise cache with the first nine terms once */
    if (cache == NULL) {
        cache_cap = 9;                         /* indices 0 .. 8 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                  /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)17;
        cache[4] = (seq_t)25;
        cache[5] = (seq_t)40;
        cache[6] = (seq_t)58;
        cache[7] = (seq_t)75;
        cache[8] = (seq_t)95;
    }

    /* if the requested value is already cached, return it */
    if ( (idx_t)n < (idx_t)cache_cap )
        return cache[(size_t)n];

    /* grow the cache to accommodate up to index n */
    if ( (size_t)n + 1 > cache_cap ) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                   /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (size_t i = 9; i <= (size_t)n; ++i) {
        /* all constants are cast to seq_t before multiplication */
        seq_t pos = ((seq_t)2) * cache[i-1] +
                    ((seq_t)3) * cache[i-3] +
                    ((seq_t)2) * cache[i-5] +
                    cache[i-7];

        seq_t neg = ((seq_t)2) * cache[i-2] +
                    ((seq_t)3) * cache[i-4] +
                    ((seq_t)2) * cache[i-6];

        cache[i] = pos - neg;    /* pos ≥ neg for all n ≥ 9 */
    }

    return cache[(size_t)n];
}
