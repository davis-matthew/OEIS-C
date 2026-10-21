/*
 * A258347  Expansion of Product_{k>=1} 1/(1-x^k)^(k*(k+1)).
 * https://oeis.org/A258347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coeff of x^n in ∏_{k≥1} (1-x^k)^(-k(k+1))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 24, u64 n <= 58, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    /* allocate two working arrays of size n+1 */
    seq_t *dp1 = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *dp2 = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp1 || !dp2) { /* memory failure unlikely; abort if it occurs */
        return (seq_t)0;
    }
    dp1[(size_t)0] = (seq_t)1;

    /* iterate over all k up to n */
    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* exponent a = k*(k+1) for this factor */
        seq_t a_exp = (seq_t)k * ((seq_t)k + (seq_t)1);
        idx_t mmax = n / k;
        if (mmax == (idx_t)0) continue;

        /* copy current dp into dp2 as base for this factor */
        for (size_t i = 0; i <= (size_t)n; ++i) dp2[i] = dp1[i];

        /* iterate over multiplicity m for this part size k */
        seq_t coeff = (seq_t)1;              /* coeff for m=0 is 1, but we start from m=1 */
        for (idx_t m = (idx_t)1; m <= mmax; ++m) {
            /* compute binomial coefficient C(a_exp + m - 1, m) */
            coeff = coeff * (a_exp + (m - (idx_t)1)) / (seq_t)m;

            idx_t step = k * m;
            for (idx_t s = step; s <= n; ++s) {
                dp2[s] += dp1[s - step] * coeff;
            }
        }

        /* swap dp1 and dp2 for next iteration */
        seq_t *tmp = dp1;
        dp1 = dp2;
        dp2 = tmp;
    }

    seq_t result = dp1[n];
    free(dp1);
    free(dp2);
    return result;
}
