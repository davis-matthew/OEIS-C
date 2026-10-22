/*
 * A370564  Size of the group Q_2*_/(Q_2*)^n, where Q_2 is the field of 2-adic
 *          numbers.
 * https://oeis.org/A370564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n for odd n; otherwise a(n) = n * 2^(v2(n)+1) where v2(n)
 *            is the exponent of 2 in n
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* n must be >= 1 */
    if ((n & 1ULL) == 1ULL) /* odd */
        return (seq_t)n;

    /* Compute exponent of 2 in n (v2(n)). */
    idx_t e = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0ULL) {
        tmp >>= 1;
        e++;
    }

    /* 2^(e+1) */
    seq_t pow2 = pow_uint(2ULL, e + 1);

    return (seq_t)n * pow2;
}
