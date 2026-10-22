/*
 * A344207  Dirichlet g.f.: Product_{k>=2} 1 / (1 - k^(-s))^binomial(k+6,7).
 * https://oeis.org/A344207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Dirichlet coeffs of Product_{k>=2} 1/(1 - k^{-s})^C(k)
 *            where C(k)=binom(k+6,7). The function computes a(n) by dynamic
 *            programming using the multiplicative expansion of each factor
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: binomial() from utils */

// Note: `idx_t` and `seq_t` are provided by the build system.
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;                 /* offset 1 */

    /* Allocate two arrays of seq_t for DP */
    seq_t *prev = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!prev || !curr) {                 /* unlikely, but guard */
        if (prev) free(prev);
        if (curr) free(curr);
        return 0;
    }

    /* Initialize */
    for (idx_t i = 0; i <= n; ++i) prev[i] = 0;
    prev[1] = 1;                           /* a(1) */

    /* Main DP: iterate over each integer k ≥ 2 */
    for (idx_t k = 2; k <= n; ++k) {
        /* Copy previous state to current */
        for (idx_t i = 1; i <= n; ++i) curr[i] = prev[i];

        /* Binomial coefficient C(k) = binom(k+6,7) */
        seq_t ck = binomial(k + 6, 7);

        /* Enumerate powers of k */
        idx_t pow = k;            /* k^1 */
        seq_t w   = ck;           /* weight for t=1 */
        idx_t t   = 1;

        while (pow <= n) {
            /* Add contributions from using k^t */
            for (idx_t m = pow; m <= n; m += pow) {
                curr[m] += w * prev[m / pow];
            }

            /* Prepare next power and weight */
            ++t;
            if (pow > n / k) break;   /* avoid overflow of power */

            pow *= k;                  /* k^(t+1) */

            /* w = w * (ck + t - 1) / t  ;   integer division is exact */
            seq_t numerator = w * (ck + (seq_t)(t - 1));
            w = numerator / (seq_t)t;
        }

        /* Swap buffers for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Result is in prev[n] */
    seq_t result = prev[n];

    /* Free allocated memory */
    free(prev);
    free(curr);

    return result;
}
