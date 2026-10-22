/*
 * A370269  Coefficient of x^n in the expansion of 1/( (1-x) * (1-x^2)^3 )^n.
 * https://oeis.org/A370269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(3*n + k - 1, k) * binomial(2*n
 *            - 2*k - 1, n - 2*k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return 1;

    seq_t sum = 0;
    /* floor(n/2) */
    idx_t max_k = n >> 1;

    for (idx_t k = 0; k <= max_k; ++k)
    {
        /* binomial(3*n + k - 1, k) */
        seq_t c1 = binomial(3*n + k - 1, k);

        /* binomial(2*n - 2*k - 1, n - 2*k) */
        seq_t c2 = binomial(2*n - 2*k - 1, n - 2*k);

        sum += c1 * c2;
    }

    return sum;
}
