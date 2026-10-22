/*
 * A370622  Coefficient of x^n in the expansion of ( (1-x)^2 / (1-x-x^2)^3
 *          )^n.
 * https://oeis.org/A370622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*n+k-1,k) *
 *            binomial(2*n-k-1,n-2*k) (coefficients of x^n in
 *            ((1-x)^2/(1-x-x^2)^3)^n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        /* binomial(3*n + k - 1, k) */
        seq_t t1 = binom(3 * n + k - 1, k);
        /* binomial(2*n - k - 1, n - 2*k) */
        seq_t t2 = binom(2 * n - k - 1, n - 2 * k);
        sum += t1 * t2;
    }
    return sum;
}
