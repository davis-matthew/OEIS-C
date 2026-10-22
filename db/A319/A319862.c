/*
 * A319862  Triangle read by rows, 0 <= k <= n: T(n,k) is the denominator of
 *          the k-th Bernstein basis polynomial of degree n evaluated at the
 *          interval midpoint t = 1/2; numerator is A319861.
 * https://oeis.org/A319862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, tabl
 * Formula:   T(n,k) = denominator of binomial(n,k)/2^n a(m) = T(r,k) where m
 *            = r*(r+1)/2 + k (flattened triangle)
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/seq_pow.h"   /* seq_pow */

seq_t a(idx_t m)
{
    /* find row r such that r*(r+1)/2 <= m < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrtl(8.0L * (long double)m + 1.0L) - 1.0L) / 2.0L);
    while ((r + 1) * (r + 2) / 2 <= m) r++;
    while (r * (r + 1) / 2 > m) r--;

    /* column index k within row r */
    idx_t k = m - r * (r + 1) / 2;

    /* exponent of 2 in binomial(r,k) using Kummer's theorem */
    unsigned int pc_r   = __builtin_popcountll((unsigned long long)r);
    unsigned int pc_k   = __builtin_popcountll((unsigned long long)k);
    unsigned int pc_rk  = __builtin_popcountll((unsigned long long)(r - k));
    unsigned int v2 = pc_k + pc_rk - pc_r;

    /* exponent of 2 in the denominator 2^r / 2^v2 */
    int denom_exp = (int)(r - v2);

    /* T(r,k) = 2^denom_exp */
    return seq_pow((seq_t)2, (idx_t)denom_exp);
}
