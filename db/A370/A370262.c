/*
 * A370262  Triangle read by rows: T(n, k) = binomial(n+k, n-k)/(2*k + 1) *
 *          (2*n + 1)^k.
 * https://oeis.org/A370262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r+k, r-k)/(2*k + 1) * (2*r + 1)^k where (r, k)
 *            are determined from n by reading the triangle T(r, k) row‑by‑row
 *            (row = 0,1,2,…) and column k = 0…r. The first few values are
 *            1,1,1,1,5,5,1,14,49,49,1,…
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 42, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* integer square root of a 128‑bit unsigned value.
   Returns floor(sqrt(x)).  */
static idx_t isqrt_uint128(unsigned __int128 x)
{
    idx_t lo = 0, hi = ((idx_t)1 << 34);   /* 2^34 > sqrt(2^67) */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;   /* hi < lo and hi satisfies hi*hi <= x */
}

seq_t a(idx_t n)
{
    /* Determine row r and column k from the linear index n. */
    unsigned __int128 tmp = (unsigned __int128)8 * n + 1;
    idx_t t = isqrt_uint128(tmp);
    idx_t r = (t - 1) / 2;                /* row */

    unsigned __int128 tri128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t tri = (idx_t)tri128;
    idx_t k = n - tri;                    /* column */

    /* Compute T(r, k).  Avoid premature truncation by dividing
       after the full product has been formed. */
    seq_t bin = binomial(r + k, r - k);   /* binomial coefficient */
    idx_t denom_val = 2 * k + 1;
    seq_t denom = (seq_t)denom_val;

    seq_t pow_term = 1;
    seq_t base = (seq_t)(2 * r + 1);
    idx_t kk = k;
    while (kk--) {
        pow_term = pow_term * base;
    }

    seq_t product = bin * pow_term;
    return product / denom;
}
