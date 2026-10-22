/*
 * A365414  a(n) = sigma(6*n+4). Sum of the divisors of 6*n+4, n >= 0.
 * https://oeis.org/A365414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = sigma(6*n+4)
 * Range:     u8 n <= 21, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;      /* m = n */
    m = m * 6 + 4;           /* m = 6*n + 4 */
    return sigma_of((idx_t)m);
}
