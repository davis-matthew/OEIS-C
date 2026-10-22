/*
 * A370280  Coefficient of x^n in the expansion of 1/( (1-x)^2 - x )^n.
 * https://oeis.org/A370280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k-1,k) * binomial(3*n+k-1,n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Accumulate the sum by iteratively updating each binomial using recurrence relations. */

#include <stdint.h>
#include "../utils/binomial.h"      /* provides seq_t binomial(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* B1 = C(n+k-1, k)   (initially k=0: C(n-1,0)=1)             */
    seq_t B1 = (seq_t)1;

    /* B2 = C(3*n+k-1, n-k)   (initially k=0: C(3*n-1, n))      */
    seq_t B2 = binomial((idx_t)(3 * n - 1), n);

    seq_t ans = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        ans += B1 * B2;

        if (k == n)
            break;

        /* Update B1: C(n+k, k+1) from C(n+k-1, k) */
        B1 = B1 * (seq_t)(n + k) / (seq_t)(k + 1);

        /* Update B2: C(3*n+k, n-k-1) from C(3*n+k-1, n-k)   */
        /* B2 *= (3*n + k) * (n - k) / ((2*n + 2*k + 1) * (2*n + 2*k)) */
        B2 = B2 * (seq_t)(3 * n + k) * (seq_t)(n - k)
            / ((seq_t)(2 * n + 2 * k + 1) * (seq_t)(2 * n + 2 * k));
    }

    return ans;
}
