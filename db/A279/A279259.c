/*
 * A279259  Smallest positive integer m such that m, m+1, m+2, m+3 are
 *          divisible by 2n+1, 2n+3, 2n+5, 2n+7 respectively.
 * https://oeis.org/A279259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1 + lcm(2*n+1,2*n+3,2*n+5,2*n+7))/2
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 151, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t a1 = (seq_t)n * 2 + 1;      /* 2*n+1   */
    seq_t a2 = a1 + 2;                /* 2*n+3   */
    seq_t a3 = a1 + 4;                /* 2*n+5   */
    seq_t a4 = a1 + 6;                /* 2*n+7   */

    seq_t lcm = a1;
    seq_t g;

    g = gcd(lcm, a2);
    lcm = (lcm / g) * a2;

    g = gcd(lcm, a3);
    lcm = (lcm / g) * a3;

    g = gcd(lcm, a4);
    lcm = (lcm / g) * a4;

    return (a1 + lcm) / 2;
}
