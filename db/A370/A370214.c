/*
 * A370214  Coefficient of x^n in the expansion of ( (1+x) / (1-x^3)^2 )^n.
 * https://oeis.org/A370214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(2*n + k - 1, k) * binomial(n, n
 *            - 3*k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t maxk = n / 3;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = binomial(2 * n + k - 1, k);
        seq_t term2 = binomial(n, 3 * k);
        sum += term1 * term2;
    }
    return sum;
}
