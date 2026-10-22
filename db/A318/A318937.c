/*
 * A318937  a(n) = 16 times the sum of the cubes of the divisors of 2*n+1.
 * https://oeis.org/A318937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16 * σ₃(2*n + 1) (sum of cubes of divisors of 2*n+1)
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 318, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    /* m = 2*n + 1 (always odd) */
    idx_t m = (idx_t)((seq_t)2 * (seq_t)n + 1);

    /* σ₃(m) = sum of d³ over all divisors d of m */
    seq_t sigma = sigma_k(m, (uint8_t)3);

    /* a(n) = 16 * σ₃(m) */
    return sigma * (seq_t)16;
}
