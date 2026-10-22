/*
 * A358877  Triangle read by rows: T(n,k) is the number of cubes of side
 *          length k that can be placed inside a cube of side length n without
 *          overlap, 1 <= k <= n.
 * https://oeis.org/A358877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = floor(n/k)^3, a(n) = T(r,c) where r = smallest integer
 *            with r(r+1)/2 ≥ n, c = n - r(r-1)/2 (1 ≤ c ≤ r)
 * Range:     u8 n <= 21, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 ≤ x ≤ 2^64‑1. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = 4294967296ULL;          /* 2^32, > sqrt(2^64−1)          */
    while (lo + 1 < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)                                 /* undefined, return 0 */
        return 0;

    /* Find r: first row that contains the n‑th term. */
    idx_t sq = isqrt_u64(8 * n + 1);            /* floor(sqrt(8n+1))       */
    idx_t r  = (sq - 1) / 2;                    /* floor((sqrt(8n+1)-1)/2) */

    /* Adjust if the inequality r(r+1)/2 < n holds. */
    if ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;

    /* Compute column index c. */
    idx_t prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - prev_tri;                     /* 1 ≤ c ≤ r                */

    /* Value of the term: floor(r/c)^3. */
    idx_t q = r / c;
    seq_t q_seq = (seq_t)q;
    seq_t val = q_seq * q_seq * q_seq;

    return val;
}
