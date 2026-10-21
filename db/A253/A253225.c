/*
 * A253225  Number of (n+1) X (2+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minimum minus antidiagonal minimum nondecreasing
 *          horizontally and diagonal maximum minus antidiagonal maximum
 *          nondecreasing vertically.
 * https://oeis.org/A253225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19 + (377 + 627*n) * 2^(n-6) for n ≥ 6, a(1)=47,
 *            a(2)=132, a(3)=306, a(4)=742, a(5)=1775
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 24, u64 n <= 54, u128 n <= 117,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)47;
    if (n == 2) return (seq_t)132;
    if (n == 3) return (seq_t)306;
    if (n == 4) return (seq_t)742;
    if (n == 5) return (seq_t)1775;

    /* n ≥ 6 */
    seq_t term = (seq_t)377 + (seq_t)627 * (seq_t)n;
    seq_t power_of_two = pow2((idx_t)(n - 6));   /* 2^(n-6) */
    return term * power_of_two + (seq_t)19;
}
