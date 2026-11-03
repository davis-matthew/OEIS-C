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
 * Validated: standard, pass, 94 terms through n = 5343 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Convert the flat index n (1‑indexed) to triangular coordinates.
   row >= 1, col >= 1 and col <= row. */
static void triangle_coords(idx_t n, idx_t *row, idx_t *col)
{
    /* Find minimal r such that r*(r+1)/2 >= n.
       Use binary search in __int128 to avoid overflow. */
    idx_t low = 1, high = 1;
    while ((__int128)high * (high + 1) / 2 < n)
        high <<= 1;               /* double until the range is enough */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    *row = low;
    /* first index of this row = (row-1)*row/2 + 1 */
    idx_t first_of_row = ((__int128)(low - 1) * low) / 2 + 1;
    *col = n - first_of_row + 1;
}

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
