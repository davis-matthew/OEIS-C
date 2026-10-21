/*
 * A277988  a(n) = 352*2^n + 34.
 * https://oeis.org/A277988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 352*2^n + 34
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    return (seq_t)352 * pow2(n) + (seq_t)34;
}
