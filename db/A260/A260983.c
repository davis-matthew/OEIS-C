/*
 * A260983  Coefficients of the mock theta function gammabar(q).
 * https://oeis.org/A260983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in the generating function 1 +
 *            2∑_{k≥1} q^{k(k+1)/2} * (1−q^2)(1−q^4)…(1−q^{2k−2})(1−q^k) /
 *            (1−q^3)(1−q^6)…(1−q^{3k})
 * Range:     i8 n <= 15, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computed by double‑loop DP for each term of the series.  */

#include <stddef.h>  /* size_t */
#include <stdint.h>  /* uint64_t */
#include <stdlib.h>  /* calloc, free, realloc */

seq_t a(idx_t n)
{
    /* static cache of computed coefficients */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of allocated terms */

    if (n >= cache_size) {
        /* recompute the whole sequence up to n (inclusive)            */
        idx_t new_size = n + 1;            /* terms 0 .. n */
        seq_t *new_cache = (seq_t *)calloc((size_t)new_size, sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* out of memory – return 0 */

        new_cache[0] = (seq_t)1;          /* a(0) = 1 */

        /* temporary array for DP; size need only up to n                */
        seq_t *dp = (seq_t *)calloc((size_t)new_size, sizeof(seq_t));
        if (!dp) {
            free(new_cache);
            return (seq_t)0;
        }

        /* compute each term of the series: 1 + 2 Σ_{k=1}^{n} ...        */
        for (idx_t k = 1; k < new_size; ++k) {
            /* shift amount for this term */
            seq_t T = ((seq_t)k) * ((seq_t)k + 1) / 2;
            if (T >= (seq_t)new_size)          /* no contribution to indexes ≤ n */
                continue;

            idx_t L = (idx_t)(new_size - 1 - (idx_t)T);   /* max internal index   */
            int L_int = (int)L;

            /* clear dp up to L */
            for (int i = 0; i <= L_int; ++i) dp[i] = (seq_t)0;
            dp[0] = (seq_t)1;

            /* numerator factors (1 − q^{2t}) for t = 1 .. k−1  */
            for (idx_t t = 1; t <= k - 1; ++t) {
                int a = (int)(2 * t);
                if (a > L_int) continue;
                for (int s = L_int; s >= a; --s) dp[s] -= dp[s - a];
            }

            /* numerator factor (1 − q^{k}) */
            if (k <= L_int) {
                int a = (int)k;
                for (int s = L_int; s >= a; --s) dp[s] -= dp[s - a];
            }

            /* denominator factors 1/(1−q^{3t}) for t = 1 .. k */
            for (idx_t t = 1; t <= k; ++t) {
                int d = (int)(3 * t);
                if (d > L_int) continue;
                for (int s = d; s <= L_int; ++s) dp[s] += dp[s - d];
            }

            /* accumulate contribution to final sequence */
            for (int i = 0; i <= L_int; ++i) {
                if (dp[i] == 0) continue;
                idx_t idx = (idx_t)T + (idx_t)i;
                new_cache[idx] += (seq_t)2 * dp[i];
            }
        }

        free(dp);
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
