/*
 * A360282  Triangle read by rows. T(n, k) = (1/2) * binomial(2*(n - k + 1), n
 *          - k + 1) * binomial(2*n - k, k - 1) for n > 0, T(0, 0) = 1.
 * https://oeis.org/A360282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 124, u64 n <= 413,
 *            u128 n <= 1325, big any n
 * Validated: standard, pass, 71 terms through n = 1299 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A360282: triangle read by rows T(n,k) with
   T(0,0)=1 and for n>0:
     T(n,k) = (1/2) * binomial(2*(n-k+1), n-k+1) * binomial(2*n-k, k-1)
   The overall sequence a(n) is obtained by reading the triangle row by
   row, left to right. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
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

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Determine which row 'n' falls into:
       cumulative terms up to row r: (r+1)(r+2)/2 > n  */
    idx_t sq = (idx_t)floor_sqrt((seq_t)(8ULL * n + 1ULL));
    idx_t r  = (sq - 1) / 2;

    /* Position within the row (0‑based): */
    idx_t pre = r * (r + 1) / 2;
    idx_t pos = n - pre;          /* pos ∈ [0, r] */

    if (pos == 0)                 /* first entry in a non‑zero row */
        return 0;

    /* Convert row/position to T(n,k) indices: */
    idx_t k = pos;                /* 1 ≤ k ≤ r */
    idx_t u = r - k + 1;          /* u = n - k + 1 */

    seq_t bin1 = binomial(2 * u, u); /* binomial(2u, u)  */
    bin1 /= 2;                    /* divide by 2 */
    seq_t bin2 = binomial(u + r - 1, k - 1);

    return bin1 * bin2;
}
