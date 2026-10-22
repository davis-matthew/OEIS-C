/*
 * A386781  a(n) = n^3*sigma_7(n).
 * https://oeis.org/A386781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^3 * sigma_7(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 7127,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t sigma = sigma_k(n, 7);   /* sum of seventh powers of divisors of n */
    return nn * nn * nn * sigma;
}
