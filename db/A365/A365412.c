/*
 * A365412  a(n) = sigma(6*n+2). Sum of the divisors of 6*n+2, n >= 0.
 * https://oeis.org/A365412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(6*n+2)
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    idx_t m = 6*n + 2;
    return sigma_of(m);
}
