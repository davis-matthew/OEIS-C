/*
 * A323771  Expansion of e.g.f. 2*exp(x/(2 - 2*x))*sinh(sqrt(5)*x/(2 -
 *          2*x))/sqrt(5).
 * https://oeis.org/A323771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n - 5)*a(n-1) - (6*n^2 - 22*n + 19)*a(n-2) +
 *            (n-3)*(n-2)*(4*n - 9)*a(n-3) - (n-4)*(n-3)^2*(n-2)*a(n-4).
 *            Initial values: a(0)=0, a(1)=1, a(2)=3, a(3)=14. This
 *            implementation memoizes results; each call is O(n) amortized
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache         = NULL;
    static idx_t cache_size     = 0;   /* allocated length */
    static idx_t cached_upto    = 0;   /* largest index computed */

    /* (1) Allocate or grow cache to at least n+1 elements */
    if (cache == NULL) {
        /* First call: allocate at least 4 slots for the base values */
        idx_t alloc = n + 1;
        if (alloc < 4) alloc = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * alloc);
        cache_size = alloc;
    } else if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        cache_size = new_size;
    }

    /* (2) Initialise base values 0..3 if the cache just grew or is new */
    /* Using unconditional assignments is safe; values may be overwritten. */
    cache[0] = (seq_t)0;
    if (cache_size > 1) cache[1] = (seq_t)1;
    if (cache_size > 2) cache[2] = (seq_t)3;
    if (cache_size > 3) cache[3] = (seq_t)14;

    /* (3) Update cached_upto to the greatest already initialized index */
    if (cached_upto < 3) cached_upto = 3;
    if (cached_upto > cache_size - 1) cached_upto = cache_size - 1;

    /* (4) Compute any missing values up to n */
    if (cached_upto < n) {
        idx_t start = cached_upto + 1;
        if (start < 4) start = 4;  /* base values already set */

        for (idx_t i = start; i <= n; ++i) {
            /* coefficients are small enough to fit in idx_t */
            seq_t t12 = cache[i-1] * ( (seq_t)((4*i)-5) )
                       - cache[i-2] * ( (seq_t)(6*i*i - 22*i + 19) );

            seq_t t34 = cache[i-3] * ( (seq_t)((i-3)*(i-2)*(4*i-9) ) )
                       - cache[i-4] * ( (seq_t)((i-4)*(i-3)*(i-3)*(i-2)) );

            cache[i] = t12 + t34;
        }
        cached_upto = n;
    }

    return cache[n];
}
