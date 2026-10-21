/*
 * A278123  a(n) = 247*2^n - 300.
 * https://oeis.org/A278123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 247*2^n - 300
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t pow2n = seq_pow((seq_t)2, n);

    /* Multiply by 247 */
    seq_t val = seq_mul(pow2n, (seq_t)247);

    /* Subtract 300 */
    val -= (seq_t)300;

    return val;
}
