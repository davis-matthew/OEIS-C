/*
 * A360855  Array read by antidiagonals: T(m,n) is the number of triangles in
 *          the rook graph K_m X K_n.
 * https://oeis.org/A360855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the antidiagonal reading of the array T(m,n) =
 *            m*binom(n,3) + n*binom(m,3). The first terms (for n = 1,2,3,...)
 *            are 0,0,0,1,0,1,4,2,2,4,10,
 * Range:     u8 n <= 57, u16 n <= 914, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(x)) for 0 <= x <= UINT64_MAX using binary search
   and __uint128_t to avoid overflow. */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* binom(x,3) = x*(x-1)*(x-2)/6, returned as seq_t */
static seq_t binom3(idx_t x)
{
    if (x < 3) return (seq_t)0;
    return (seq_t)x * (x - 1) * (x - 2) / 6;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find diagonal d such that (d-1)*d/2 >= n.
       Use quadratic formula with integer sqrt. */
    idx_t tmp = 1 + 8 * n;
    idx_t sq = floor_sqrt_u64(tmp);
    idx_t d = (1 + sq) / 2;
    /* Fix potential off‑by‑one errors */
    while ((__uint128_t)d * (d - 1) / 2 < n) ++d;
    while (d > 2 && (__uint128_t)(d - 1) * (d - 2) / 2 >= n) --d;

    /* Number of terms before diagonal d */
    idx_t prev = (d - 2) * (d - 1) / 2;
    /* Position within the diagonal (0‑based) */
    idx_t pos = n - prev - 1;
    /* Coordinates (m, n2) on this anti‑diagonal */
    idx_t m  = pos + 1;
    idx_t n2 = d - m;

    /* Compute T(m,n2) = m*binom(n2,3) + n2*binom(m,3) */
    seq_t term1 = (seq_t)m * binom3(n2);
    seq_t term2 = (seq_t)n2 * binom3(m);
    return term1 + term2;
}
