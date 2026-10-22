/*
 * A380115  a(n) = max{A380114(n, k) : k = 0..n}.
 * https://oeis.org/A380115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * binomial(n-1, floor((n-1)/2)) (OEIS A380115)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Compute 2^n */
    seq_t pow2_val = pow2(n);

    /* Compute the central binomial coefficient binomial(n-1, floor((n-1)/2)) */
    idx_t m = n - 1;
    idx_t k = m / 2;               /* floor((n-1)/2) */
    seq_t bin_val = binomial(m, k);

    /* Return the product */
    return bin_val * pow2_val;
}
