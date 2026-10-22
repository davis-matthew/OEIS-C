/*
 * A368606  Irregular triangular array T, read by rows: T(n,k) = number of
 *          sums |x-y| + |y-z| = k, where x,y,z are in {1,2,...,n} and x <= y
 *          and y >= z.
 * https://oeis.org/A368606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   For a given index n, find row r = ⌊√(n‑1)⌋ + 1 and offset k = n
 *            – (r‑1)² – 1. Then a(n) = T(r,k) where T(r,k) =
 *            Σ_{y=⌈(k+2)/2⌉}^{r} c(y,k) with c(y,k) = (k+1) if k ≤ y–2
 *            (2y–k–1) if k ≥ y–1. This sum is evaluated in closed form
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* integer square root: floor(√x) */
static idx_t isqrt64(idx_t x)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);  /* 2^32 > √(2^64) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k */
    idx_t r = isqrt64(n - 1) + 1;            /* row index (≥1) */
    idx_t r_minus1 = r - 1;
    unsigned __int128 sq = (unsigned __int128)r_minus1 * r_minus1;
    idx_t k = n - (idx_t)sq - 1;             /* offset in row, 0‑based */

    /* y ranges start from ceil((k+2)/2) */
    idx_t y_start = (k + 3) / 2;              /* ceil((k+2)/2) */

    /* Determine the split point between the two formulas */
    idx_t a_end = (k + 1 < r) ? (k + 1) : r;  /* min(r, k+1) */
    idx_t b_start = (y_start > k + 2) ? y_start : (k + 2);   /* max(y_start, k+2) */

    seq_t result = 0;

    /* Sum for y in [y_start, a_end] using 2y - k - 1 */
    if (y_start <= a_end) {
        idx_t cnt1 = a_end - y_start + 1;
        unsigned __int128 sumY = (unsigned __int128)(y_start + a_end) * cnt1 / 2;
        unsigned __int128 sum1 = 2 * sumY - (unsigned __int128)(k + 1) * cnt1;
        result += (seq_t)sum1;
    }

    /* Sum for y in [b_start, r] using k + 1 */
    if (b_start <= r) {
        idx_t cnt2 = r - b_start + 1;
        unsigned __int128 sum2 = (unsigned __int128)(k + 1) * cnt2;
        result += (seq_t)sum2;
    }

    return result;
}
