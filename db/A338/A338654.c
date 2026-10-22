/*
 * A338654  T(n, k) = 2^n * Product_{j=1..k} (j/2)^((-1)^(j - 1)). Triangle
 *          read by rows, for 0 <= k <= n.
 * https://oeis.org/A338654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the row index such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 and
 *            let k = n - r(r+1)/2 (0 ≤ k ≤ r). Write k = 2m or 2m+1. Then
 *            a(n) = T(r,k) where if k = 2m : a(n) = 2^(r‑2m) * C(2m,m) if k =
 *            2m+1 : a(n) = 2^(r‑2m‑1) * (2m+1) * C(2m,m) (C(2m,m) is the
 *            binomial coefficient). The sequence is the tri...
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 520, u64 n <= 2040,
 *            u128 n <= 8101, big any n
 * Helpers:   binom_2m_m, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binom_2m_m.h"

/* integer square root of a 128‑bit unsigned integer */
static inline idx_t isqrt_u128(unsigned __int128 v)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64); /* exclusive hi */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (idx_t)(lo - 1);   /* floor sqrt */
}

seq_t a(idx_t n)
{
    /* find row r and column k */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t root = isqrt_u128(val);
    idx_t r = (root - 1) / 2;

    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    while (tri > n) {
        --r;
        tri = (unsigned __int128)r * (r + 1) / 2;
    }
    while (((unsigned __int128)(r + 1) * (r + 2) / 2) <= n) {
        ++r;
        tri = (unsigned __int128)r * (r + 1) / 2;
    }

    idx_t k = n - tri;
    idx_t m = k / 2;   /* floor(k/2) */

    seq_t bin = binom_2m_m(m);

    idx_t exp = r - 2 * m - (k & 1);   /* exponent of 2, always >= 0 */
    seq_t result = pow2(exp) * bin;

    if (k & 1) {   /* odd k: multiply by (2m+1) which equals k */
        result *= (seq_t)k;
    }

    return result;
}
