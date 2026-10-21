/*
 * A285262  Expansion of Product_{k>=1} ((1-x^(4*k))/(1-x^k))^k.
 * https://oeis.org/A285262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in the product Product_{k>=1} ((1 -
 *            x^(4k))/(1 - x^k))^k which simplifies to Product_{k>=1} (1 + x^k
 *            + x^{2k} + x^{3k})^k The function computes a(n) via dynamic
 *            programming on the truncated product up to exponent n. The
 *            result is returned as a seq_t (unsigned integer of...
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 48, u64 n <= 124, u128 n <= 330,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* allocate two work arrays for dp and dp next */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp || !next) return 0; /* allocation failure */

    /* initialize dp[0] = 1, others 0 */
    memset(dp, 0, sz * sizeof(seq_t));
    dp[0] = 1;

    for (idx_t k = 1; k <= n; ++k) {
        /* f[t] = coefficient of y^t in (1 + y + y^2 + y^3)^k,
           with y = x^k.  We compute it by iterating k variables,
           each with choices 0,1,2,3. */
        idx_t mmax = n / k;
        size_t fsz = (size_t)mmax + 1;
        seq_t *f = (seq_t *)malloc(fsz * sizeof(seq_t));
        if (!f) { free(dp); free(next); return 0; }
        memset(f, 0, fsz * sizeof(seq_t));
        f[0] = 1;

        /* iterative DP over the k variables */
        for (idx_t var = 1; var <= k; ++var) {
            for (idx_t s = mmax; s <= mmax; --s) {
                seq_t val = f[s];
                if (!val) continue;
                if (s + 1 <= mmax) f[s + 1] += val;
                if (s + 2 <= mmax) f[s + 2] += val;
                if (s + 3 <= mmax) f[s + 3] += val;
            }
        }

        /* convolution of current dp with f, shifted by multiples of k */
        memset(next, 0, sz * sizeof(seq_t));
        for (idx_t exp = 0; exp <= n; ++exp) {
            seq_t val = dp[exp];
            if (!val) continue;
            for (idx_t t = 0; t <= mmax; ++t) {
                idx_t shift = k * t;
                if (exp + shift > n) break;
                next[exp + shift] += val * f[t];
            }
        }

        free(f);
        /* swap dp and next for next iteration */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = dp[n];
    free(dp);
    free(next);
    return result;
}
