/*
 * A261720  Array of pyramidal (3-dimensional figurate numbers) read by
 *          antidiagonals.
 * https://oeis.org/A261720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(n,k) = k*(k+1)*((k-1)*n + 3)/6, where (n,k) are the row
 *            and column indices of the position n in the array read by
 *            antidiagonals (rows and columns start at 1)
 * Range:     u8 n <= 52, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 128‑bit prefix sum s*(s-1)/2 for a diagonal index s */
static inline unsigned __int128 tri_prefix(idx_t s)
{
    return (unsigned __int128)s * (s - 1) / 2;
}

/* Convert linear index to (row,column) on a table read by antidiagonals.
   The table has entries T(row, col) for row, col >= 1. */
static inline void antidiagonal_from_index(idx_t idx, idx_t *row, idx_t *col)
{
    if (idx == 0) {
        *row = 0;
        *col = 0;
        return;
    }

    /* Find the smallest s with s*(s-1)/2 >= idx.  s is the sum row+col. */
    idx_t low  = 1;
    idx_t high = 2;
    while (tri_prefix(high) < idx) {
        if (high > (UINT64_MAX >> 1)) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;                /* double until bound holds */
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (tri_prefix(mid) >= idx)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                                   /* sum row+col */
    idx_t start_of_diag = (idx_t)(tri_prefix(s - 1) + 1);
    idx_t off = idx - start_of_diag;                 /* zero‑based offset on diag */
    *row = s - 1 - off;                              /* row number  >= 1 */
    *col = 1 + off;                                  /* column number >= 1 */
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    idx_t row, col;
    antidiagonal_from_index(n, &row, &col);

    /* Compute T(row,col) = col*(col+1)*((col-1)*row + 3)/6 safely. */
    unsigned __int128 c  = col;
    unsigned __int128 r  = row;
    unsigned __int128 val =
        c * (c + 1) * ((c - 1) * r + 3) / 6;
    return (seq_t)val;
}
