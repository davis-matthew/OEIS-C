/*
 * A338432  Triangle read by rows: T(n, k) = (n - k + 1)^2 + 2*k^2, for n >=
 *          1, and k = 1, 2, ..., n.
 * https://oeis.org/A338432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 9876, u32 n <= 9876, u64 n <= 9876,
 *            u128 n <= 9876, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A338432 – Triangle read by rows:
   a(n) = T(row, col) where
   row r = ceil((‑1 + sqrt(1+8n))/2),
   col k = n – r(r–1)/2,
   T(r,k) = (r‑k+1)² + 2·k².  */
#include <stdint.h>

static idx_t triangular_row(idx_t n)
{
    idx_t r = 1;
    /* compute r until r*(r+1)/2 >= n
       use 128‑bit arithmetic to avoid overflow */
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* locate the position (row, column) in the triangle */
    idx_t r = triangular_row(n);
    idx_t before = (unsigned __int128)r * (r - 1) / 2;   /* elements before row r */
    idx_t k = n - before;                               /* 1‑based column */
    idx_t m = r - k + 1;                                 /* compute (n‑k+1) */

    /* compute the value (m‑k+1)² + 2·k² in seq_t */
    seq_t m_s = (seq_t)m;
    seq_t k_s = (seq_t)k;
    return m_s * m_s + (seq_t)2 * k_s * k_s;
}
