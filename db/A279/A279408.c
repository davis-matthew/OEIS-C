/*
 * A279408  Triangle read by rows: T(n,m) (n>=m>=1) = domination number for
 *          kings' graph on an n X m toroidal board.
 * https://oeis.org/A279408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r, c) where T(n,m) = ceil( max( m*ceil(n/3),
 *            n*ceil(m/3) ) / 3 )
 * Range:     u8 n <= 856, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the triangle read by rows: rows correspond to n>=m>=1, offset 1. */

#include <stdint.h>

/* integer square root: floor(sqrt(x)) for unsigned 64-bit values */
static idx_t floor_sqrt_idx(idx_t x)
{
    if (x == 0) return 0;
    idx_t lo = 0, hi = 1;
    /* grow hi until hi*hi > x */
    while ((__int128)hi * hi <= x) {
        hi <<= 1;
        if (hi == 0) { /* overflow, cap at max */
            hi = x;
            break;
        }
    }
    while (hi - lo > 1) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; behaviour below is arbitrary */

    /* Find row k such that k(k+1)/2 >= n */
    idx_t s = floor_sqrt_idx(1 + 8 * n);          /* floor sqrt of 1+8n */
    /* ceil sqrt */
    idx_t t = ( (__int128)s * s < (idx_t)1 + 8 * n ) ? s + 1 : s;
    idx_t k = t / 2;                              /* row number (1-based) */

    /* Compute index within row */
    idx_t half = ( (__int128)k * (k - 1) ) / 2;
    idx_t r = n - half;                           /* 1 <= r <= k */
    idx_t m = r;                                   /* column number (1-based) */

    /* Compute T(k, m) = ceil( max(m*ceil(k/3), k*ceil(m/3)) / 3 ) */
    seq_t kk = k;
    seq_t mm = m;
    seq_t ceil_k_div3 = (kk + 2) / 3;              /* ceil(k/3) */
    seq_t ceil_m_div3 = (mm + 2) / 3;              /* ceil(m/3) */
    seq_t s1 = mm * ceil_k_div3;
    seq_t s2 = kk * ceil_m_div3;
    seq_t max_s = s1 > s2 ? s1 : s2;
    seq_t result = (max_s + 2) / 3;                /* ceil(max_s / 3) */

    return result;
}
