/*
 * A334811  Length of shortest overlap-free binary word having n periods.
 * https://oeis.org/A334811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   * a(1) = 1 * a(2) = 2 * a(3) = 5 * a(n) = (17 * 2^(2n-7) + 2) /
 *            3 for n >= 4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   mul_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;

    /* Exponent for the power of two: 2n - 7 */
    idx_t exp = 2 * n - 7;

    /* Compute 2^(2n-7) */
    seq_t pw = seq_pow((seq_t)2, exp);

    /* Compute 17 * 2^(2n-7) + 2 */
    seq_t numer = mul_small(pw, 17);
    numer = numer + (seq_t)2;

    /* Final result: divide by 3 */
    return numer / (seq_t)3;
}
