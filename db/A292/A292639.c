/*
 * A292639  Rank of (3+r)*n when all the numbers (3-r)*j and (3+r)*k, where r
 *          = sqrt(5), j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor( n*(7 + 3*sqrt(5))/2 )
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for 64‑bit unsigned integers.
   Returns floor(sqrt(x)).  Works for x up to (2^64−1). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1 << 32, mid, ans = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        unsigned __int128 m2 = (unsigned __int128)mid * mid;
        if (m2 <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety, outside OEIS offset */

    /* A = 14*n,  B = 6*n   (both fit in seq_t for the required range) */
    seq_t A = (seq_t)14 * n;
    seq_t B = (seq_t)6  * n;

    /* sq = 5 * B^2  (64‑bit product – fits if n is modest, e.g. n≤10 000) */
    idx_t sq = (idx_t)5 * ((idx_t)B * (idx_t)B);

    /* floor(B*sqrt(5)) = sqrt_floor(5*B^2) */
    idx_t sqrt_s = isqrt_u64(sq);

    /* floor((A + sqrt_s)/4) = floor( A/4 + sqrt_s/4 ) = floor( (14n + 6n*sqrt(5))/4 ) */
    seq_t part = (A + (seq_t)sqrt_s) / 4;

    return n + part;
}
