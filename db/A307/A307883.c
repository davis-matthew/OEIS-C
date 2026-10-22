/*
 * A307883  Square array read by descending antidiagonals: T(n, k) where
 *          column k is the expansion of 1/sqrt(1 - 2*(k+1)*x + ((k-1)*x)^2).
 * https://oeis.org/A307883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where * s = ⌊(√(8*n + 1) – 1)/2⌋, * row = n –
 *            s*(s + 1)/2, * col = s – row, * T(r,c) = Σ_{j=0..r} c^j *
 *            (binomial(r,j))^2
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 130, u64 n <= 342,
 *            u128 n <= 934, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square root floor of a 64‑bit unsigned value. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;  /* hi < lo, hi is floor(sqrt(n)) */
}

/* Compute T(row,col) = Σ_{j=0..row} col^j * (binomial(row,j))^2 */
static seq_t compute_T(idx_t row, idx_t col)
{
    seq_t sum   = 0;
    seq_t pow_c = 1;          /* col^0 */
    for (idx_t j = 0; j <= row; ++j) {
        seq_t bin   = binomial(row, j);   /* binomial(row,j) */
        seq_t term  = pow_c * bin * bin;  /* col^j * bin^2   */
        sum += term;
        pow_c = pow_c * (seq_t)col;      /* next power of col */
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Determine diagonal index s: number of preceding elements = s*(s+1)/2 */
    idx_t s = (isqrt_u64(8 * n + 1) - 1) / 2;

    idx_t total = s * (s + 1) / 2;  /* elements before diagonal s */
    idx_t d     = n - total;        /* position inside diagonal (0 ≤ d ≤ s) */

    idx_t row = d;
    idx_t col = s - d;
    return compute_T(row, col);
}
