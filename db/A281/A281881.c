/*
 * A281881  Triangle read by rows: T(n,k) (n>=1, 2<=k<=n+1) is the number of
 *          k-sequences of balls colored with at most n colors such that
 *          exactly one ball is of a color seen previously in the sequence.
 * https://oeis.org/A281881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(k,2) * n! / (n+1-k), where * n and k are derived
 *            from the index n: * let row be the smallest integer such that
 *            row*(row+1)/2 ≥ n; * let k = n - (row-1)*row/2 + 1 (so k runs 2
 *            … row+1). * The sequence lists the triangle T(row,k) read
 *            row‑by‑row
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 75, u64 n <= 188,
 *            u128 n <= 557, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Find the row number (1‑based) that contains the n‑th term. */
static inline idx_t _row_of(idx_t pos) {
    /* Approximate with double, then adjust for rounding errors. */
    double d = sqrt((double)pos * 8.0 + 1.0);
    idx_t row = (idx_t)((d - 1.0) / 2.0);           /* floor value */

    /* Guarantee row*(row+1)/2 ≥ pos. */
    while (( (__int128)row * (row + 1) ) / 2 < pos)
        ++row;
    return row;
}

seq_t a(idx_t n) {
    /* Locate the row and column within that row. */
    idx_t row  = _row_of(n);
    idx_t prev = (row * (row - 1)) / 2;      /* terms before this row */
    idx_t j    = n - prev;                    /* 1‑based position in row */
    idx_t k    = j + 1;                       /* actual column (2…row+1) */

    /* binomial(k,2) = k*(k-1)/2 */
    seq_t binom = (seq_t)k * (seq_t)(k - 1) / 2;

    /* Compute n!/(n+1-k)! as product from i = row+2-k to row. */
    idx_t start = row + 2 - k;
    seq_t prod = 1;
    for (idx_t i = start; i <= row; ++i) {
        prod *= (seq_t)i;
    }

    return binom * prod;
}
