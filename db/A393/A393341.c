/*
 * A393341  Number of Cayley permutations of size n avoiding 123 and 321.
 * https://oeis.org/A393341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*2^n - n - 7 - 3*n^2 for n>0, a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 99,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t value = pow2(n);      /* 2^n */
    value = value * 6;          /* 6*2^n */
    value = value - (seq_t)n;   /* subtract n */
    value = value - (seq_t)7;   /* subtract 7 */
    seq_t sq = (seq_t)n * (seq_t)n;   /* n^2 */
    value = value - sq * 3;     /* subtract 3*n^2 */
    return value;
}
