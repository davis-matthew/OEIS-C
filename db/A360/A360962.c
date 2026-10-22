/*
 * A360962  Square array T(n,k) = k*((3+6*n)*k - 1)/2; n>=0, k>=0, read by
 *          antidiagonals upwards.
 * https://oeis.org/A360962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the square array T(n,k) = k*((3+6*n)*k - 1)/2
 *            in anti‑diagonal order. For a given index n: d =
 *            floor((sqrt(8*n+1)-1)/2) // diagonal containing n m = n -
 *            d*(d+1)/2 // offset on that diagonal T = m * (((3 + 6*(d-m))*m)
 *            - 1) / 2 // value at coordinates (d-m, m) Thus a(n) = T. The
 *            f...
 * Range:     u8 n <= 40, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Floor square root of an unsigned __int128 value, result fits in idx_t. */
static idx_t floor_sqrt_uint128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)-1;          /* maximum 64‑bit value */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/* Return the n-th term of OEIS A360962. */
seq_t a(idx_t n)
{
    /* Compute d = floor((sqrt(8*n+1)-1)/2).  Handle 8*n+1 without overflow. */
    unsigned __int128 tmp = (unsigned __int128)8 * n + (unsigned __int128)1;
    idx_t s = floor_sqrt_uint128(tmp);
    idx_t d = (s - (idx_t)1) >> 1;

    /* m = n - d*(d+1)/2 */
    idx_t tri = (d * (d + (idx_t)1)) >> 1;
    idx_t m   = n - tri;          /* 0 <= m <= d */

    /* If k = m = 0, the array entry is 0. */
    if (m == 0) return (seq_t)0;

    idx_t nIdx = d - m;           /* n-coordinate of the entry */

    /* Compute: value = k * ((3 + 6*nIdx) * k - 1) / 2, using 128‑bit arithmetic
       to avoid intermediate overflow. */
    unsigned __int128 part   = (unsigned __int128)3 + (unsigned __int128)6 * nIdx;
    unsigned __int128 inner  = part * m - (unsigned __int128)1;
    unsigned __int128 total  = (unsigned __int128)m * inner;
    unsigned __int128 val128 = total / (unsigned __int128)2;

    return (seq_t)val128;
}
