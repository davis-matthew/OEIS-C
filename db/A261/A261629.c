/*
 * A261629  Expansion of Product_{k>=0} 1/(1-x^(4*k+1))^2.
 * https://oeis.org/A261629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = coefficient of x^n in
   Π_{k≥0} 1/(1 - x^(4k+1))^2 (OEIS A261629) */
#include <stdlib.h>

/* External types: seq_t (unsigned, arbitrary‑precision) and idx_t (uint64_t) */
seq_t a(idx_t n)
{
    /* Static cache for previous results */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;   /* number of elements in cache */

    /* Return cached value if already computed */
    if (cached_size > 0 && n < (idx_t)(cached_size - 1))
        return cache[n];

    /* --- Compute up to n from scratch ------------------------------------ */
    idx_t size = n + 1ULL;
    seq_t *dp  = (seq_t *)calloc((size_t)size, sizeof(seq_t));
    seq_t *t1  = (seq_t *)calloc((size_t)size, sizeof(seq_t));
    seq_t *t2  = (seq_t *)calloc((size_t)size, sizeof(seq_t));

    if (!dp || !t1 || !t2) {
        /* Allocation failed */
        free(dp); free(t1); free(t2);
        return 0;
    }

    dp[0] = 1;                     /* base case: coefficient of x^0 */

    /* Process parts m = 1,5,9,… up to n */
    for (idx_t m = 1; m <= n; m += 4) {
        /* t1[i] = Σ_{t≥0} dp[i - t*m]   (cumulative sum over residues modulo m) */
        for (idx_t i = 0; i < size; ++i) {
            t1[i] = dp[i];
            if (i >= m)
                t1[i] += t1[i - m];
        }
        /* t2[i] = Σ_{t≥0} t1[i - t*m]   (weighted sum gives the new coefficient) */
        for (idx_t i = 0; i < size; ++i) {
            t2[i] = t1[i];
            if (i >= m)
                t2[i] += t2[i - m];
        }
        /* Prepare for the next part: dp <- t2, free the old dp buffer */
        seq_t *tmp = dp;
        dp = t2;
        t2 = tmp;                  /* now t2 points to the old dp buffer */
    }

    /* dp now holds the final sequence up to n */
    /* If we already had a cache, free it */
    if (cache)
        free(cache);

    cache = dp;                    /* store for future calls */
    cached_size = size;            /* cache now has 'size' elements */

    /* Clean up temporary buffers */
    free(t1);
    free(t2);                      /* t2 now holds the old dp buffer, which has been freed above */

    return cache[n];
}
