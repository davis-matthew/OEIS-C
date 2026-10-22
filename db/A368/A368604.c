/*
 * A368604  Triangular array T, read by rows: T(n,k) = number of sums
 *          |x-y|+|y-z| = k, where x,y,z are in {1,2,...,n} and x < y and y >
 *          z.
 * https://oeis.org/A368604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k), where - r is the row containing position n: r
 *            minimal with r(r+1)/2 >= n - k is the column within that row: k
 *            = n - (r-1)r/2 (1 ≤ k ≤ r) - T(r, k) = ⌊(k-1)²/4⌋ + k·(r-k) This
 *            equals the number of triples (x, y, z) with 1 ≤ x, y, z ≤ r, x <
 *            y, y > z, and |x-y|+|y-z| = k+1
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n) {
        ++r;
    }
    ++r;   /* now r is the correct row number */

    /* Column index k (1‑based) within that row. */
    idx_t k = n - (r - 1) * r / 2;   /* 1 ≤ k ≤ r */

    /* Compute ⌊(k-1)²/4⌋. */
    seq_t km1 = (seq_t)(k - 1);
    seq_t part1 = km1 * km1 / 4;

    /* Compute k·(r-k). */
    seq_t part2 = (seq_t)k * (seq_t)(r - k);

    return part1 + part2;
}
