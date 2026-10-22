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
 * Helpers:   compute_A_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_A_at.h"

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
