/*
 * A391636  Integers k such that the sum of the products of each digit of k
 *          and the number obtained by removing that digit is equal to k.
 * https://oeis.org/A391636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   sequence of integers k such that A088116(k) = k, i.e. k = sum
 *            over digits d of (d * (k with that digit removed)). The only
 *            solutions are 36·10^j and 1314·10^j (j ≥ 0), sorted ascending.
 *            Thus a(1)=36, a(2)=360, a(3)=1314, a(4)=3600, a(5)=13140, etc
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1)               /* first term */
        return (seq_t)36;

    if ((n & 1) == 0) {       /* even n: 36·10^(n/2) */
        idx_t exp = n / 2;
        return (seq_t)36 * pow10(exp);
    } else {                  /* odd n≥3: 1314·10^((n-3)/2) */
        idx_t exp = (n - 3) >> 1;   /* (n-3)/2 */
        return (seq_t)1314 * pow10(exp);
    }
}
