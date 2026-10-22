/*
 * A337314  a(n) is the number of n-digit positive integers with exactly four
 *          distinct base 10 digits.
 * https://oeis.org/A337314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 189 * (4^n - 4*3^n + 3*2^(n+1) - 4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow4  = seq_pow((seq_t)4, n);         /* 4^n   */
    seq_t pow3  = seq_pow((seq_t)3, n);         /* 3^n   */
    seq_t pow21 = seq_pow((seq_t)2, n + 1);     /* 2^(n+1) */

    /* temp = 4^n + 3*2^(n+1) - 4 */
    seq_t temp = pow4 + (pow21 * (seq_t)3) - (seq_t)4;

    /* expr = temp - 4*3^n  */
    seq_t expr = temp - (pow3 * (seq_t)4);

    return expr * (seq_t)189;
}
