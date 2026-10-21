/*
 * A255250  Array T(n, m) of numbers of points of a square lattice in the
 *          first octant covered by a circular disk of radius n (centered at
 *          any lattice point taken as origin) with ordinate y = m.
 * https://oeis.org/A255250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* A255250.
 * Let T(n,m) = floor(sqrt(n² – m²)) – (m–1), 0 ≤ m ≤ floor(n/√2).
 * The sequence a(n) is obtained by listing the rows T(0,0),
 * [T(1,0),T(1,1)], [T(2,0),T(2,1),T(2,2)], … and flattening them.
 *
 * The function below finds the row r that contains index n,
 * determines the column c, and returns the corresponding value
 * of T(r,c).  All arithmetic is performed in seq_t (or idx_t when
 * it is safe), so the implementation works for fixed‑width and
 * arbitrary‑precision builds alike. */

static inline idx_t row_len(idx_t r)
{
    /* Length of row r: 1 + floor(r/√2) = 1 + floor( sqrt(r² / 2) ) */
    seq_t sq   = (seq_t)r * (seq_t)r;          /* r²   (exact for all ranges used) */
    seq_t half = sq / (seq_t)2;                /* floor(r²/2)                     */
    seq_t fs   = floor_sqrt(half);             /* floor(sqrt(half))               */
    return 1 + (idx_t)fs;
}

seq_t a(idx_t n)
{
    idx_t cum = 0;          /* number of terms before the current row   */
    idx_t r   = 0;          /* current row                              */

    /* Find the row that contains index n.  cum + row_len(r) is the
       cumulative size up to the end of row r.  The loop stops when
       the next row would exceed n. */
    while (cum + row_len(r) <= n) {
        cum += row_len(r);
        ++r;
    }

    /* Column within the row (0‑based). */
    idx_t c = n - cum;

    /* Compute T(r,c) = floor( sqrt(r² – c²) ) – (c–1) = floor_sqrt(r² – c²) – c + 1 */
    seq_t r_sq = (seq_t)r * (seq_t)r;
    seq_t c_sq = (seq_t)c * (seq_t)c;
    seq_t diff = r_sq - c_sq;          /* r² – c² (≥ 0) */
    seq_t fs   = floor_sqrt(diff);     /* floor(sqrt(r² – c²)) */
    seq_t val  = fs - (seq_t)c + (seq_t)1;

    return val;
}
