/*
 * A390846  Expansion of e.g.f. exp( x^2/2 + x^3/6 + x^4/24 ).
 * https://oeis.org/A390846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) * a(n-2) + (n-1)*(n-2)*a(n-3)/2 +
 *            (n-1)*(n-2)*(n-3)*a(n-4)/6 with a(0)=1, a(1)=0, a(2)=1, a(3)=1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

/* Memoisation cache */
static seq_t *cache        = NULL;   /* array of computed terms  */
static idx_t  cache_size  = 0;      /* number of stored terms   */

seq_t a(idx_t n)
{
    /* Direct values for the base cases */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)0;
            case 2: return (seq_t)1;
            case 3: return (seq_t)1;
        }
    }

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)4 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache_size = 4;
    }

    /* Extend the cache if necessary */
    if (n >= cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed – abort silently */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* term1 = (i-1) * a(i-2) */
            seq_t term1 = (seq_t)(i - 1) * cache[i - 2];

            /* term2 = (i-1)*(i-2)/2 * a(i-3) */
            seq_t t2 = (seq_t)(i - 1) * (i - 2);
            t2 = t2 / 2;
            seq_t term2 = t2 * cache[i - 3];

            /* term3 = (i-1)*(i-2)*(i-3)/6 * a(i-4) */
            seq_t t3 = (seq_t)(i - 1) * (i - 2);
            t3 = t3 / 2;          /* = (i-1)*(i-2)/2 */
            t3 = t3 * (i - 3);    /* now (i-1)*(i-2)*(i-3)/2 */
            t3 = t3 / 3;          /* = (i-1)*(i-2)*(i-3)/6 */
            seq_t term3 = t3 * cache[i - 4];

            cache[i] = term1 + term2 + term3;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
