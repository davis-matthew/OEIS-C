/*
 * A356469  a(n) = [(n + 1)/(1 - 1/r)] - [n - n/r] where r = sqrt(2) and []
 *          denotes the floor function.
 * https://oeis.org/A356469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n + 1)/(1 - 1/r)) - floor(n - n/r) * with r =
 *            sqrt(2) and [] indicating floor. The * equivalent integer‐only
 *            expression is * a(n) = floor((n+1)*(2+√2)) - floor(n*(1-1/√2))
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* compute floor((n+1)*(2+√2)) */
    seq_t m = (seq_t)n + 1;
    seq_t m_sq = m * m;                     /* (n+1)^2 */
    seq_t sqrt_term = floor_sqrt( m_sq * 2 ); /* floor((n+1)*√2) */
    seq_t alpha_term = 2 * m + sqrt_term;   /* floor((n+1)*(2+√2)) */

    /* compute floor(n*(1-1/√2)) = n - ceil(n/√2) */
    seq_t n_sq = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t n_sq_div2 = n_sq / 2;                /* floor(n^2/2) */
    seq_t sqrt2_floor = floor_sqrt(n_sq_div2); /* floor(n/√2) */
    seq_t ceil_sqrt2 = sqrt2_floor;
    if (n > 0)                                     /* ceil(n/√2) = floor + 1 for n>0 */
        ++ceil_sqrt2;
    seq_t beta_term = (seq_t)n - ceil_sqrt2;       /* floor(n*(1-1/√2)) */

    return alpha_term - beta_term;
}
