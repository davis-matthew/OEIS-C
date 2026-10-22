/*
 * A382117  a(n) = sum (-1)^(((x - 1)*(y - 1))/4), where x and y are coprime
 *          positive integers, equidistant from n, such that x <= y.
 * https://oeis.org/A382117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over coprime pairs (x,y) equidistant from n of
 *            (-1)^(((x-1)*(y-1))/4)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    long long sum = 0;
    for (idx_t d = 0; d < n; ++d)
    {
        idx_t x = n - d;
        idx_t y = n + d;
        seq_t sx = (seq_t)x;
        seq_t sy = (seq_t)y;
        if (gcd(sx, sy) != (seq_t)1)
            continue;
        seq_t prod = (sx - 1) * (sy - 1);           /* product divisible by 4 */
        seq_t parity = ((prod & (seq_t)7) >> 2) & (seq_t)1; /* parity of prod/4 */
        if (parity == (seq_t)1)
            --sum;  /* odd exponent => -1 */
        else
            ++sum;  /* even exponent => +1 */
    }
    return (seq_t)sum;
}
