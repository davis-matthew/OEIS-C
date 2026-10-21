/*
 * A272027  a(n) = 3*sigma(n).
 * https://oeis.org/A272027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*A000203(n)
 * Range:     u8 n <= 35, u16 n <= 6299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = 3 * sigma(n) for n >= 1 */
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);   /* sum of divisors of n */
    s *= (seq_t)3;           /* multiply by 3 */
    return s;
}
