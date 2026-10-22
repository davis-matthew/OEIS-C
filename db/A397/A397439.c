/*
 * A397439  Growth function of the reflection group of a three-dimensional
 *          orthoscheme with distinct edge lengths.
 * https://oeis.org/A397439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=9, a(n)=2*a(n-1) for n>=3 (so
 *            a(n)=9⋅2^(n-2) for n>=2)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)9;
    /* n >= 3 */
    seq_t val = pow2(n - 2);          /* 2^(n-2) */
    val *= (seq_t)9;                  /* 9*2^(n-2) */
    return val;
}
