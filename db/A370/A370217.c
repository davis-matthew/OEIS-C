/*
 * A370217  Coefficient of x^n in the expansion of ( (1+x)^2 / (1-x^3)^3 )^n.
 * https://oeis.org/A370217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A370217: a(n) = Sum_{k=0..floor(n/3)} binomial(3*n+k-1, k) * binomial(2*n, n-3*k) */

/* The function uses the helper provided by the build environment:
   binomial(idx_t n, idx_t k)  (in "../utils/binomial.h")      */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    idx_t maxk = n / 3;          /* floor(n/3) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term1 = binomial(3 * n + k - 1, k);
        seq_t term2 = binomial(2 * n, n - 3 * k);
        sum += term1 * term2;
    }

    return sum;
}
