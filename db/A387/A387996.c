/*
 * A387996  One third the number of solid partitions of n with 3 parts.
 * https://oeis.org/A387996
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9k² + α₍r₎ k + β₍r₎, where k = n/6 (integer division), r
 *            = n%6, α = {-2,1,4,7,10,13}, β = {0,0,0,2,2,5}. This satisfies
 *            the OEIS formula G(q)=q³·(q²+2)/((1‑q)(1‑q²)(1‑q³))
 * Range:     u8 n <= 32, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* values below offset are unused */

    /* k = floor(n/6), r = n mod 6 */
    idx_t k = n / 6;
    int r = (int)(n % 6);

    static const int alpha[6] = { -2, 1, 4, 7, 10, 13 };
    static const int beta[6]  = {  0, 0, 0, 2,  2,  5 };

    /* compute using signed 64‑bit intermediate to handle negative α */
    long long kk = (long long)k;
    long long val = 9LL * kk * kk + (long long)alpha[r] * kk + (long long)beta[r];

    return (seq_t)val;
}
