/*
 * A295821  Number of coprime pairs (a,b) with -n <= a <= n, -2 <= b <= 2.
 * https://oeis.org/A295821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n + 5 - (-1)^n for n >= 1; a(0) = 2
 * Range:     u8 n <= 41, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 2;
    /* (-1)^n is 1 when n is even, -1 when n is odd.  */
    if (n & 1ULL)
        return (seq_t)n * 6 + 6;   /* 6*n + 5 - (-1) = 6*n + 6 */
    else
        return (seq_t)n * 6 + 4;   /* 6*n + 5 - 1  = 6*n + 4 */
}
