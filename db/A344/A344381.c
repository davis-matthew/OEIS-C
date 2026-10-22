/*
 * A344381  Triangle read by rows: T(n,k) = (n+2) * (Sum_{i=k..n} i!) / ((k+2)
 *          * k!) for 0 <= k <= n with T(i,j) = 0 if j < 0 or i < j.
 * https://oeis.org/A344381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r = floor( (sqrt(8*n+1)-1)/2 ) k = n -
 *            r*(r+1)/2 T(n,k) is defined by T(k,k) = 1 T(k+1,k) = k+3 T(n,k)
 *            = (n+2) * (T(n-1,k) - T(n-2,k)) for n >= k+2 This works for all
 *            n >= 0
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* Helper: integer square root of a 128‑bit value */
static idx_t isqrt_u128(unsigned __int128 x)
{
    /* The maximum possible sqrt(8*n+1) for n <= UINT64_MAX
       is below 2^34, so we use a fixed upper bound. */
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 34;  /* 2^34 > sqrt(8*UINT64_MAX+1) */
    idx_t res = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

/* Compute the triangle entry T(n,k) using the recurrence. */
static seq_t T_of(idx_t n, idx_t k)
{
    if (n == k) {
        return (seq_t)1;
    }
    if (n == k + 1) {
        return (seq_t)(k + 3);          /* k+3 fits in idx_t, cast to seq_t */
    }
    seq_t prev2 = (seq_t)1;              /* T(k,k)   */
    seq_t prev1 = (seq_t)(k + 3);        /* T(k+1,k) */
    for (idx_t i = k + 2; i <= n; ++i) {
        seq_t cur = (seq_t)(i + 2) * (prev1 - prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    unsigned __int128 tmp = (unsigned __int128)8 * n + 1;
    idx_t r = (isqrt_u128(tmp) - 1) / 2;
    idx_t k = n - r * (r + 1) / 2;
    return T_of(r, k);
}
