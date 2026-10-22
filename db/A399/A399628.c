/*
 * A399628  Number of finite ordered sequences of nonsingleton subsets of an
 *          n-set such that the successive unions strictly increase and the
 *          final union is the full set.
 * https://oeis.org/A399628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F_n(0) where F_n(n) = 1 F_n(r) = Σ_{d=1..n-r} C(n-r,d) *
 *            b(r,d) * F_n(r+d) with b(r,1) = 2^r - 1, b(r,d≥2) = 2^r. See the
 *            OEIS entry of A399628 for details
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Special cases */
    if (n <= 1) return (seq_t)0;
    /* Allocate arrays for dynamic programming */
    seq_t *F = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *pow2 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!F || !pow2) {
        /* Allocation failed – return 0 as a safe fallback */
        return (seq_t)0;
    }

    const seq_t one = (seq_t)1;
    const seq_t zero = (seq_t)0;

    /* Precompute powers of two */
    pow2[0] = one;
    for (idx_t i = 1; i <= n; ++i) {
        pow2[i] = pow2[i - 1] * (seq_t)2;
    }

    /* Base value */
    F[n] = one;

    /* Main DP: compute F[r] from r = n-1 down to 0 */
    for (idx_t r = n; r-- > 0;) {
        seq_t sum = zero;
        idx_t maxd = n - r;
        for (idx_t d = 1; d <= maxd; ++d) {
            seq_t bin = binomial(maxd, d);          /* C(n-r, d) */
            seq_t mult = (d == 1) ? (pow2[r] - one) : pow2[r];
            mult = mult * bin;
            mult = mult * F[r + d];
            sum = sum + mult;
        }
        F[r] = sum;
    }

    seq_t result = F[0];

    /* Free allocated memory */
    free(F);
    free(pow2);

    return result;
}
