/*
 * A378550  a(n) = 1 if sigma(n) >= 2*n-1, otherwise 0.
 * https://oeis.org/A378550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [sigma(n) >= 2*n-1]
 * Range:     u8 n <= 107, u16 n <= 9731, u32 n <= 9731, u64 n <= 9731,
 *            u128 n <= 9731, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

/* Return 1 if the sum of divisors of n is at least 2*n-1, otherwise 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; guard against n==0 */

    seq_t sig  = sigma_of(n);             /* sum of divisors of n */
    seq_t rhs  = ((seq_t)n << 1) - 1;     /* 2*n - 1 */

    return (sig >= rhs) ? (seq_t)1 : (seq_t)0;
}
