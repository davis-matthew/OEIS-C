/*
 * A372458  Coefficient of x^n in the expansion of 1 / ( (1-x) * (1-x-x^2)^2
 *          )^n.
 * https://oeis.org/A372458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binom(2*n+k-1,k) * binom(4*n-k-1,n-2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    idx_t maxk = n / 2;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* Compute the two binomial coefficients using the provided helper. */
        seq_t first  = binomial(2 * n + k - 1, k);
        seq_t second = binomial(4 * n - k - 1, n - 2 * k);

        /* Add the product to the running total. */
        sum += first * second;
    }

    return sum;
}
