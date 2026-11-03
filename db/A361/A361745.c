/*
 * A361745  Square array of circular Delannoy numbers A(i,j) (i >= 0, j >= 0)
 *          read by antidiagonals.
 * https://oeis.org/A361745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,j) where (i,j) are obtained by reading the square
 *            array of circular Delannoy numbers A(i,j) by antidiagonals.
 *            A(0,j) = A(i,0) = 1 A(i,j) = Σ_{k=0..min(i,j)}
 *            binomial(i,k)·binomial(j,k)·k·2^k (i,j ≥ 1)
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t compute_A_at(idx_t i, idx_t j)
{
    if (i == 0 || j == 0) return 1;

    idx_t m = (i < j) ? i : j;
    seq_t binom_i = 1;
    seq_t binom_j = 1;
    seq_t pow2   = 1;
    seq_t sum    = 0;

    for (idx_t k = 0; k <= m; ++k)
    {
        if (k > 0)
        {
            /* Update binomials for next k using the recurrence
               C(n,k+1) = C(n,k) * (n-k) / (k+1)  */
            binom_i = binom_i * (seq_t)(i - k + 1) / (seq_t)k;
            binom_j = binom_j * (seq_t)(j - k + 1) / (seq_t)k;
            pow2   = pow2 * (seq_t)2;
        }
        seq_t term = binom_i * binom_j * (seq_t)k * pow2;
        sum += term;
    }
    return sum;
}

/* Compute (i,j) coordinates on antidiagonals from index n */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t lo = 0, hi = n;
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t d = lo;
    idx_t start = d * (d + 1) / 2;
    idx_t offset = n - start;
    *i = offset;
    *j = d - offset;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return compute_A_at(i, j);
}
