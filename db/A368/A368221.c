/*
 * A368221  Table read by downward antidiagonals: T(n,k) is the number of
 *          tilings of the n X k grid up to horizontal reflection by two tiles
 *          that are each fixed under horizontal reflection.
 * https://oeis.org/A368221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let (i,j) be the pair obtained from n by reading the table * of
 *            T(i,j) along downward antidiagonals. Then * a(n) = (2^(i*j) +
 *            2^(ceil(i/2)*j)) / 2 * where ceil(i/2) is (i+1)/2 in integer
 *            arithmetic
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* find the antidiagonal number s such that s*(s-1)/2 >= n */
    idx_t s = 2;
    while ((s * (s - 1)) / 2 < n)
        ++s;

    /* first index on this antidiagonal */
    idx_t start = ((s - 2) * (s - 1)) / 2 + 1;
    idx_t offset = n - start;          /* 0‑based in this diagonal */
    idx_t i = offset + 1;              /* row count */
    idx_t j = s - i;                   /* column count */

    /* number of symmetric rows */
    idx_t rows_half = (i + 1) / 2;

    idx_t total_cells = i * j;
    idx_t sym_cells   = rows_half * j;

    seq_t term1 = pow2(total_cells);
    seq_t term2 = pow2(sym_cells);

    seq_t result = (term1 + term2) / 2;
    return result;
}
