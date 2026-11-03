/*
 * A285679  Positions of 2 in A285677.
 * https://oeis.org/A285679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*floor((n-1)*phi) - n + 4
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9793 (2026-11-01)
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

/* Helper: compute floor(k*phi) for integer k (k >= 0) using only integer arithmetic */
static inline seq_t floor_k_phi(idx_t k)
{
    seq_t kk  = (seq_t)k;            /* cast to seq_t */
    seq_t sq  = kk * kk;             /* k^2 */
    seq_t t   = sq * 5;              /* 5*k^2 */
    seq_t s   = floor_sqrt(t);       /* floor(sqrt(5*k^2)) = floor(k*sqrt5) */
    seq_t fp  = (kk + s) / 2;        /* floor((k + k*sqrt5)/2) = floor(k*phi) */
    return fp;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset starts at 1 */
    idx_t k = n - 1;                         /* k = n-1 */
    seq_t fp   = floor_k_phi(k);              /* floor(k*phi) */
    seq_t result = (seq_t)3 * fp - (seq_t)(k + 1) + (seq_t)4;
    return result;
}
