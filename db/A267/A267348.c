/*
 * A267348  Decimal equivalents of terms of A266926 interpreted as binary
 *          numbers.
 * https://oeis.org/A267348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * (2^(3 * 2^(n-4)) - 1) / 7 for n = 4..11 Formula: a(n)
 *            = 6 * (2^(3 * 2^(n-2)) - 1) / 7 for n >= 12 a(1) = 0, a(2) = 1,
 *            a(3) = 2 The sequence is implemented via big_pow(base, exp)
 *            which returns base^exp as seq_t
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* determine the exponent shift */
    idx_t shift;
    if (n <= 11)
        shift = n - 4;
    else
        shift = n - 2;

    /* compute r = 2^shift as a big integer */
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < shift; ++i)
        r <<= 1;                     /* r = r * 2 */

    /* e = 3 * r */
    seq_t e = r * (seq_t)3;

    /* compute 2^e */
    seq_t pow2e = big_pow((seq_t)2, e);

    /* a(n) = 6 * (pow2e - 1) / 7 */
    seq_t result = (pow2e - (seq_t)1) * (seq_t)6 / (seq_t)7;
    return result;
}
