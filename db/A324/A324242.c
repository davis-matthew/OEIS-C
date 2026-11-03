/*
 * A324242  Incomplete Lucas numbers: irregular triangular array L(n,k) =
 *          Sum_{j = 0..k} (n/(n-j)) * binomial(n-j, j), read by rows, with n
 *          >= 1 and 0 <= k <= floor(n/2).
 * https://oeis.org/A324242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = L(r,k), the incomplete Lucas number in row r and column k
 *            where r is the smallest integer with cum_rows(r) ≥ n and k = n -
 *            cum_rows(r-1) - 1. L(n,k) is defined by L(n,k) = Sum_{j=0..k} (
 *            n/(n-j) ) * binomial( n-j, j ) This uses the utility function
 *            binomial(idx_t n, idx_t k) from the utils...
 * Range:     u8 n <= 31, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* total number of terms up to and including row r */
static inline idx_t cum_rows(idx_t r)
{
    if (r == 0) return 0;
    idx_t t = r >> 1;                     /* floor(r/2)           */
    __uint128_t tt = (__uint128_t)t * t;   /* t² in 128 bits       */
    if ((r & 1) == 0)                     /* r even   -> r + t²   */
        return r + (idx_t)tt;
    else                                  /* r odd    -> r + t² + t */
        return r + (idx_t)tt + t;
}

seq_t a(idx_t n)
{
    /* locate the row r that contains the n-th term */
    idx_t lo = 0, hi = 1;
    while (cum_rows(hi) < n) {
        if (hi > ((idx_t)-1) >> 1) { hi = (idx_t)-1; break; }
        hi <<= 1;
    }
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum_rows(mid) >= n)
            hi = mid;
        else
            lo = mid;
    }
    idx_t r = hi;

    /* column k (0‑based) inside that row */
    idx_t prev = cum_rows(r - 1);
    idx_t k = n - prev - 1;                 /* 0 ≤ k ≤ floor(r/2) */

    /* compute L(r,k) directly by the defining sum */
    seq_t sum = 0;
    for (idx_t j = 0; j <= k; ++j) {
        /* binomial(r-j, j) is guaranteed to be non‑zero for j ≤ floor(r/2) */
        seq_t bin = binomial(r - j, j);
        seq_t term = bin * (seq_t)r;      /* multiply first, division later */
        term /= (seq_t)(r - j);           /* exact division */
        sum += term;
    }
    return sum;
}
