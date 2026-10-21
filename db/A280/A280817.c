/*
 * A280817  a(n) = A049501(sigma(n)).
 * https://oeis.org/A280817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A049501(A000203(n))
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   major_index, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/major_index.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);   /* A000203: sum of divisors of n */
    return major_index(sigma);
}
