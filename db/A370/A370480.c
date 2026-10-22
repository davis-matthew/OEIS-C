/*
 * A370480  G.f. satisfies A(x) = ( 1 + x * (A(x)^(1/3) / (1-x))^2 )^3.
 * https://oeis.org/A370480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * Σ_{k=0..n} binomial(2k+3,k) * binomial(n+k-1,n-k) /
 *            (2k+3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              (k = 0 term is zero for n ≥ 1; a(0) = 1)                            */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)          /* k = 0 contributes 0 for n ≥ 1          */
    {
        /* Compute binomial(2k+3, k)  */
        seq_t b1 = binom(2 * k + 3, k);

        /* Perform the division (3 * b1) / (2k+3) without overflow:
           let d = 2k+3, q = b1 / d, r = b1 - q*d.  Then
           (3*b1)/d = 3*q + (3*r)/d  (all intermediate values fit in seq_t) */
        idx_t d = 2 * k + 3;
        seq_t q = b1 / d;
        seq_t r = b1 - q * d;
        seq_t t2 = q * 3 + (r * 3) / d;        /* = (3*b1)/d                     */

        /* Compute binomial(n+k-1, n-k)  */
        seq_t b2 = binom(n + k - 1, n - k);

        /* Add the contribution of this k  */
        sum += t2 * b2;
    }

    return sum;  /* outer factor 3 already applied in t2 */
}
