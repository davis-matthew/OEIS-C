/*
 * A279363  Sum of 4th powers of proper divisors of n.
 * https://oeis.org/A279363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n, d<n} d⁴
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 479, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    /* Σ_{d|n} d⁴ using the provided utility */
    seq_t total = sigma_k(n, 4);

    /* Compute n⁴ in seq_t to subtract it (proper divisors only) */
    seq_t n4 = (seq_t)n;
    n4 *= n4;   /* n²  */
    n4 *= n4;   /* n⁴  */

    return total - n4;   /* proper divisors only */
}
