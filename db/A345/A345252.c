/*
 * A345252  2-1-Fibonacci cohort array, a rectangular array T(n,k) read by
 *          downward antidiagonals.
 * https://oeis.org/A345252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where the 2‑1‑Fibonacci cohort array T is read by
 *            downward antidiagonals. For the entry at (n,k) we have T(n,k) =
 *            n + F(Finv(n)+k+2) – F(Finv(n)+2). This function computes the
 *            sequence A345252
 * Range:     u8 n <= 56, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   fib, finv_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fib.h"
#include "../utils/finv_idx.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* behavior below the offset is irrelevant */

    /* Find the diagonal number s such that s*(s+1)/2 ≥ n */
    idx_t lo = 1, hi = 2;
    while (( (__uint128_t)hi * (hi + 1) / 2 ) < n)
        hi <<= 1;

    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t val = ( (__uint128_t)mid * (mid + 1) ) / 2;
        if (val >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t s = lo;

    /* Number of terms before diagonal s */
    __uint128_t cum = ( (__uint128_t)(s - 1) * s ) / 2;
    idx_t offset = (idx_t)(n - cum - 1);  /* position inside diagonal s */
    idx_t row = offset;                  /* row index (0‑based) */
    idx_t col = s - row;                 /* column index (1‑based) */

    idx_t finv = finv_idx(row);

    idx_t idx1 = finv + col + 2;  /* index for F(Finv+col+2) */
    idx_t idx2 = finv + 2;        /* index for F(Finv+2) */

    seq_t term = (seq_t)row + fib(idx1) - fib(idx2);
    return term;
}
