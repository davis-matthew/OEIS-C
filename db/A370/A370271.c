/*
 * A370271  Coefficient of x^n in the expansion of 1/( (1-x)^3 * (1-x^2)^3
 *          )^n.
 * https://oeis.org/A370271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*n + k - 1, k) *
 *            binomial(4*n - 2*k - 1, n - 2*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t maxk = n / 2;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t termA = binomial(3 * n + k - 1, k);
        seq_t termB = binomial(4 * n - 2 * k - 1, n - 2 * k);
        sum += termA * termB;
    }

    return sum;
}
