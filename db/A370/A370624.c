/*
 * A370624  Coefficient of x^n in the expansion of 1 / (1-x-x^3)^n.
 * https://oeis.org/A370624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} binomial(n+k-1, k) * binomial(2n-2k-1,
 *            n-3k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the coefficient of x^n in the expansion of 1 / (1-x-x^3)^n (OEIS A370624). */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n = 0 is a trivial case */
    if (n == 0)
        return (seq_t)1;

    idx_t kmax = n / 3;      /* floor(n/3) */
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= kmax; ++k) {
        /* binomial(n+k-1, k) */
        seq_t b1 = binomial(n + k - 1, k);

        /* binomial(2*n-2*k-1, n-3*k) */
        seq_t b2 = binomial(2 * n - 2 * k - 1, n - 3 * k);

        sum += b1 * b2;
    }
    return sum;
}
