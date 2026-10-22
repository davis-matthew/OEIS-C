/*
 * A359613  Greatest k such that a polynomial f(x) with nonnegative integral
 *          coefficients with degree at most k is irreducible if f(n) is a
 *          prime.
 * https://oeis.org/A359613
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, hard, more, nice
 * Formula:   a(n) = 3*n + ⌊(n+2)/8⌋
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = 3 * nn + (nn + (seq_t)2) / (seq_t)8;
    return res;
}
