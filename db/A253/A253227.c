/*
 * A253227  Number of (n+1) X (4+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minimum minus antidiagonal minimum nondecreasing
 *          horizontally and diagonal maximum minus antidiagonal maximum
 *          nondecreasing vertically.
 * https://oeis.org/A253227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=335,a(2)=742,a(3)=914,a(4)=1498, for n>4: a(n)=188 +
 *            1755*2^(n-5) + 60*n + 12*n^2
 * Range:     u8 none, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 335;
    if (n == 2) return 742;
    if (n == 3) return 914;
    if (n == 4) return 1498;

    /* n >= 5 */
    seq_t result = 188;
    result += (seq_t)1755 * pow2(n - 5);
    result += (seq_t)60 * n;
    result += (seq_t)12 * n * n;
    return result;
}
