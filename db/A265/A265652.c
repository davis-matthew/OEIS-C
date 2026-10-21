/*
 * A265652  Triangle read by rows: T(n,k) is the sum of the union of the
 *          divisors of n and k.
 * https://oeis.org/A265652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = sigma_of(row) + sigma_of(col) - sigma_of(gcd(row,col))
 *            where (row,col) are the triangular coordinates of the n‑th term
 *            (rows are 1‑indexed, rows are read by rows)
 * Range:     u8 n <= 1817, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   gcd, sigma_of, triangle_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"
#include "../utils/triangle_coords.h"

/* Main entry point for the sequence.  idx_t is unsigned 64‑bit,
   seq_t is an unsigned integer of arbitrary width (or arbitrary
   precision). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard for safety */

    idx_t row, col;
    triangle_coords(n, &row, &col);

    seq_t sigma_row = sigma_of(row);
    seq_t sigma_col = sigma_of(col);
    /* gcd of two unsigned 64‑bit numbers, returned as seq_t */
    seq_t g = gcd((seq_t)row, (seq_t)col);
    seq_t sigma_g = sigma_of((idx_t)g);

    return sigma_row + sigma_col - sigma_g;
}
