/*
 * A348902  G.f. A(x) satisfies: A(x) = 1 / (1 + x - 2 * x * A(4*x)).
 * https://oeis.org/A348902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = -a(n-1) + Sum_{k=0..n-1} 2^(2*k+1) * a(k) *
 *            a(n-1-k) (n >= 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache for previously computed terms */
    static seq_t *cache   = NULL;   /* array of a[0..len-1] */
    static size_t  cap    = 0;      /* allocated capacity of cache */
    static idx_t  len     = 0;      /* number of terms currently computed */

    /* Ensure capacity and compute missing terms up to n */
    if (n >= len)
    {
        /* --- Grow the cache if necessary --------------------------------- */
        size_t needed = (size_t)n + 1;
        if (cap < needed) {
            size_t new_cap = cap ? cap << 1 : 1;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        /* --- Compute terms from current length up to n ------------------- */
        for (idx_t i = len; i <= n; ++i)
        {
            if (i == 0) {          /* initial value */
                cache[0] = (seq_t)1;
                continue;
            }

            /* Compute the convolution sum with weight 2 * 4^k */
            seq_t sum   = (seq_t)0;
            seq_t pow4  = (seq_t)1;  /* 4^k, starts at k = 0 */

            for (idx_t k = 0; k < i; ++k)
            {
                seq_t prod = cache[k] * cache[i - 1 - k];   /* a(k) * a(n-1-k) */
                prod *= (seq_t)2;                           /* 2 * a(k) * a(...) */
                prod *= pow4;                               /* * 4^k */
                sum += prod;
                pow4 *= (seq_t)4;                           /* next power of 4 */
            }

            /* a(i) = sum - a(i-1) */
            cache[i] = sum - cache[i - 1];
        }

        len = (size_t)n + 1;   /* update number of computed terms */
    }

    return cache[(size_t)n];
}
