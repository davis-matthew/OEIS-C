/*
 * A357552  a(n) = sigma(n) * binomial(2*n-1,n), for n >= 1.
 * https://oeis.org/A357552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) * binomial(2*n-1, n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t s = sigma_of(n);          /* sum of divisors of n */
    idx_t two_n_minus1 = 2 * n - 1; /* compute 2*n-1 in idx_t to avoid overflow early */
    seq_t b = binomial(two_n_minus1, n); /* binomial coefficient (2n-1 choose n) */

    return s * b;
}
