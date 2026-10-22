/*
 * A335607  Rectangular array by antidiagonals: T(n,k) = floor(n + k*r), where
 *          r = sqrt(2).
 * https://oeis.org/A335607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = row + ⌊col·√2⌋, where (row,col) are the coordinates of
 *            the integer n when the array T(i,j)=⌊i+j√2⌋ is read along
 *            antidiagonals starting from (0,0). The value of ⌊k·√2⌋ is
 *            obtained exactly by the integer square‑root of 2·k²
 * Range:     u8 n <= 20, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static seq_t floor_k_sqrt2(idx_t k)
{
    /* floor(k*sqrt(2)) = floor( sqrt(2*k*k) )           */
    seq_t kk = (seq_t)k * (seq_t)k;     /* k² */
    kk = kk * (seq_t)2;                /* 2·k² */
    return floor_sqrt(kk);
}

seq_t a(idx_t n)
{
    /* Find the diagonal index d such that
         (d+1)(d+2)/2 > n  and  d*(d+1)/2 <= n.
       This guarantees that the first term of diagonal d is at
       index d*(d+1)/2 (0‑based).                                */
    idx_t d = 0;
    for (;; ++d) {
        /* (d+1)(d+2)/2 is the cumulative number of terms up to d */
        idx_t cum = (d + 1) * (d + 2) / 2;
        if (cum > n) break;
    }

    /* cumulative number of terms up to the previous diagonal */
    idx_t prev_cum = d * (d + 1) / 2;

    /* position of n within diagonal d; 0‑based  */
    idx_t pos = n - prev_cum;

    /* coordinates of the cell: row = pos, col = d – pos */
    idx_t row = pos;
    idx_t col = d - pos;

    /* Finally compute T(row,col) = row + floor(col·√2) */
    seq_t term = (seq_t)row + floor_k_sqrt2(col);
    return term;
}
