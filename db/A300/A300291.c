/*
 * A300291  Triangle T read by rows: T is used to obtain the denominators of
 *          all fractional values for x = cos(phi) and y = sin(phi) with (x,
 *          y) on the unit circle for 0 < phi < Pi/2.
 * https://oeis.org/A300291
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row, col) where row = minimal integer ≥ 2 with
 *            (row‑1)*row/2 > (n‑2), col = (n‑2) – [(row‑1)*(row‑2)/2] + 1,
 *            and T(row, col) = row² + col² if gcd(row, col) = 1 and (row+col)
 *            is odd, = 1 otherwise
 * Range:     u8 n <= 66, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_seq.h"

/* Euclidean algorithm for seq_t */

seq_t a(idx_t n)
{
    if (n < 2)        /* undefined below the offset */
        return 0;

    idx_t idx = n - 2;        /* zero‑based position in the triangle */

    /* Find the row r such that the cumulative count of terms up to row r
       exceeds idx.  Row r has r‑1 terms. */
    idx_t r = 2;
    idx_t cumulative = 1;     /* terms up to row r */
    while (cumulative <= idx) {
        ++r;
        cumulative += r - 1;
    }

    /* Column index (1‑based) within row r */
    idx_t prev_cumulative = cumulative - (r - 1);   /* terms up to row r‑1 */
    idx_t col = idx - prev_cumulative + 1;          /* 1 ≤ col ≤ r‑1 */

    /* Evaluate the triangle entry T(r, col) */
    seq_t row = (seq_t) r;
    seq_t ccol = (seq_t) col;

    bool coprime = (gcd_seq(row, ccol) == 1);
    bool odd_sum = ((r + col) & 1) != 0;   /* (row + col) is odd */

    if (coprime && odd_sum)
        return row * row + ccol * ccol;    /* row² + col² */
    else
        return 1;                         /* otherwise */
}
