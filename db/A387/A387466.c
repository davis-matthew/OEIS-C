/*
 * A387466  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of 1/sqrt(1 - 4*x - (2*k*x)^2).
 * https://oeis.org/A387466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A387466
   a(n) is the entry A(row,col) of the square array read by antidiagonals.
   Mapping from index n to indices (row, col):
     d       = ⌊(√(8n+1)−1)/2⌋              // diagonal number
     offset  = n − d·(d+1)/2                // position on the diagonal
     row     = offset
     col     = d − offset
   A(row,col) = Σ_{j=0..⌊row/2⌋} col^(2j) · binom(2·(row−j),row−j) · binom(row−j,j)
   (binom uses idx_t arguments and returns seq_t)
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

static idx_t floor_sqrt128(__int128 x)
{
    idx_t lo = 0, hi = (idx_t)~0ULL;          /* highest 64‑bit value */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

static seq_t compute_A(idx_t n, seq_t k)
{
    seq_t result = 0;
    seq_t power  = 1;               /* k^(2j)  */
    seq_t k_sq   = k * k;           /* k^2      */

    for (idx_t j = 0; j <= n / 2; ++j) {
        if (j > 0 && k_sq == 0) break;       /* further terms vanish */
        idx_t m = n - j;                     /* m = n−j    */
        seq_t bin1 = binom(2 * m, m);        /* binom(2m,m)   */
        seq_t bin2 = binom(m, j);            /* binom(m,j)    */
        seq_t term = bin1;
        term *= bin2;
        term *= power;
        result += term;
        power *= k_sq;                       /* next power  */
    }
    return result;
}

seq_t a(idx_t n)
{
    __int128 temp = (__int128)8 * n + 1;
    idx_t d = (floor_sqrt128(temp) - 1) >> 1;        /* (√(8n+1)-1)/2   */
    __int128 tri = (__int128)d * (d + 1) / 2;        /* triangular number */
    idx_t offset = n - (idx_t)tri;                   /* position on diagonal */
    idx_t row = offset;
    idx_t col = d - offset;

    return compute_A(row, (seq_t)col);
}
