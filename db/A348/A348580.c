/*
 * A348580  Expansion of e.g.f. exp(x) / (1 - sin(x)).
 * https://oeis.org/A348580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k) * A000111(k+1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* An efficient O(n²) incremental computation that uses the EGF
   recurrence exp(x)/(1 - sin x).  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently stored */

    /* If we already have the value, return it immediately */
    if (n < cache_size)
        return cache[n];

    /* Allocate / grow the cache to size n+1 (indices 0..n) */
    idx_t old_size = cache_size;
    idx_t new_size = n + 1;
    if (!cache)
        cache = (seq_t*)calloc((size_t)new_size, sizeof(seq_t));
    else
        cache = (seq_t*)realloc(cache, (size_t)new_size * sizeof(seq_t));
    cache_size = new_size;   /* now cache[0..n] exist but may be uninitialized beyond old_size */

    /* Compute new terms from old_size up to n */
    for (idx_t cur = old_size; cur <= n; ++cur)
    {
        if (cur == 0)
        {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sumPos = 0;   /* contributions with + sign (g_{odd} = +1) */
        seq_t sumNeg = 0;   /* contributions with - sign (g_{odd} = -1) */
        seq_t binom = (seq_t)1;  /* binom(cur,0) */

        for (idx_t k = 0; k < cur; ++k)
        {
            /* r = cur - k is the order of the g coefficient */
            idx_t r = cur - k;
            if ((r & 1U) == 1U)               /* only odd r contribute */
            {
                idx_t m = (r - 1U) >> 1U;     /* m = (r-1)/2   */
                seq_t val = cache[k] * binom; /* binom * a(k)  */

                if ((m & 1U) == 0U)           /* m even  -> g = -1  */
                    sumNeg += val;
                else                          /* m odd   -> g = +1  */
                    sumPos += val;
            }

            /* update binomial coefficient for next k:
               binom(n, k+1) = binom(n, k) * (n-k)/(k+1)   */
            if (k + 1 < cur)
            {
                binom = binom * (seq_t)(cur - k);
                binom = binom / (seq_t)(k + 1);
            }
        }

        /* h_cur = 1 + sumNeg - sumPos  (sumNeg ≥ sumPos for valid terms) */
        cache[cur] = (sumNeg + (seq_t)1) - sumPos;
    }

    return cache[n];
}
