/*
 * A276870  First differences of the Beatty sequence A110117 for sqrt(2) +
 *          sqrt(3).
 * https://oeis.org/A276870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*r) - floor((n-1)*r), r = sqrt(2)+sqrt(3)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 1648, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9127 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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

/* Compute S(n) = floor(n*(sqrt(2)+sqrt(3))) as a seq_t */
static inline seq_t S_of_n(idx_t m)
{
    seq_t m_seq = (seq_t)m;
    seq_t m2    = m_seq * m_seq;           /* m^2  */
    seq_t m4    = m2 * m2;                 /* m^4  */

    /* floor(m*sqrt(2))  = floor(sqrt(2*m^2)) */
    seq_t f2 = floor_sqrt( (seq_t)2 * m2 );
    /* floor(m*sqrt(3))  = floor(sqrt(3*m^2)) */
    seq_t f3 = floor_sqrt( (seq_t)3 * m2 );

    seq_t s = f2 + f3;                    /* floor part of m*(sqrt2+sqrt3) without carry */

    /* Decide whether the fractional parts add up to ≥1  (carry) */
    seq_t R   = s + (seq_t)1;
    seq_t lhs = R * R;                    /* (s+1)^2   */
    seq_t rhs = (seq_t)5 * m2;            /* 5*m^2     */
    bool carry = false;

    if (lhs > rhs) {                      /* a carry is possible */
        seq_t diff = lhs - rhs;           /* <0 is processed above */
        seq_t diff_sq = diff * diff;      /* (lhs - rhs)^2 */
        seq_t limit = (seq_t)24 * m4;     /* 24*m^4      */
        carry = diff_sq <= limit;
    }

    return s + (carry ? (seq_t)1 : (seq_t)0);
}

/* Public function required by the platform */
seq_t a(idx_t n)
{
    if (n == 0)   return 0;              /* offset security, not used by OEIS tests */

    seq_t Sn  = S_of_n(n);
    seq_t Sn1 = (n == 1) ? (seq_t)0 : S_of_n(n - 1);

    return Sn - Sn1;                     /* a(n) = S(n) - S(n-1) */
}
