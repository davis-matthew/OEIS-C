/*
 * A361877  a(n) = binomial(2*n, n) * binomial(2*n - 1, n).
 * https://oeis.org/A361877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n, n) * binomial(2*n - 1, n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;   /* a(0) = 1 */
    }
    idx_t two_n = 2 * n;
    seq_t first  = binomial(two_n, n);
    seq_t second = binomial(two_n - 1, n);
    return first * second;
}
