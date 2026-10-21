/*
 * A268839  a(n) = Sum_{j=1..10^n-1} 2^f(j) where f(j) is the number of zero
 *          digits in the decimal representation of j.
 * https://oeis.org/A268839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (9/10)*(11^n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 11^n */
    seq_t pow = big_pow((seq_t)11, (seq_t)n);
    /* (11^n - 1) / 10 */
    seq_t res = (pow - (seq_t)1) / (seq_t)10;
    /* Multiply by 9 */
    return res * (seq_t)9;
}
