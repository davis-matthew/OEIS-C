/*
 * A261638  Expansion of Product_{k>=0} (1+x^(4*k+1))^4.
 * https://oeis.org/A261638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A261638: coefficient of x^n in ∏_{k≥0} (1 + x^(4k+1))^4 */

#include <stdint.h>
#include <stdlib.h>

/* compute the nth coefficient of the product (1 + x^(4k+1))^4 for all k≥0.
   The sequence is zero‑indexed: a(0) = 1. */
seq_t a(idx_t n)
{
    /* Allocate two working arrays of length n+1. */
    seq_t *dp   = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *newdp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (dp == NULL || newdp == NULL) {
        free(dp); free(newdp);
        return (seq_t)0;          /* allocation failure – caller may ignore */
    }

    /* Initialisation – only the constant term is present. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Coefficients of (1 + x^e)^4 = 1 + 4x^e + 6x^(2e) + 4x^(3e) + x^(4e). */
    static const seq_t coeff[5] = {(seq_t)1, (seq_t)4, (seq_t)6, (seq_t)4, (seq_t)1};

    /* Iterate over all admissible exponents e = 4k + 1. */
    for (idx_t e = 1; e <= n; e += 4) {
        /* Copy current dp to newdp – the coefficient for x^0 of the new factor. */
        for (idx_t i = 0; i <= n; ++i) newdp[i] = dp[i];

        /* Add contributions for powers 1..4 of the factor. */
        for (int t = 1; t <= 4; ++t) {
            idx_t shift = t * e;
            if (shift > n) break;
            seq_t c = coeff[t];
            for (idx_t i = shift; i <= n; ++i) {
#if defined(__SIZEOF_INT128__)
                /* Use 128‑bit arithmetic to avoid overflow on uint64_t. */
                newdp[i] = (seq_t)(((__uint128_t)dp[i - shift] * c) + newdp[i]);
#else
                newdp[i] += dp[i - shift] * c;
#endif
            }
        }

        /* Prepare for next factor – swap pointers. */
        seq_t *tmp = dp; dp = newdp; newdp = tmp;
    }

    seq_t result = dp[n];
    free(dp);
    free(newdp);
    return result;
}
