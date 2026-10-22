/*
 * A355173  The Fuss-Catalan triangle of order 1, read by rows. Related to
 *          binary trees.
 * https://oeis.org/A355173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A355173: Fuss‑Catalan triangle of order 1 (row‑major read).
   For an index n ≥ 0 we find the unique row r and column k such that
   n = r*(r+1)/2 + k, 0 ≤ k ≤ r.
   Then
        a(0) = 1,
        a(n) =
             0                    if k == 0,
             (r-k+2) * C(r+k-1, k-1) / (r+1)   otherwise.
   The binomial is evaluated via the utility function binomial(idx_t,idx_t).
   All arithmetic is performed in seq_t. */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n = 0 gives the first element of the triangle (row 0, column 0). */
    if (n == 0u)
    {
        return (seq_t)1;
    }

    /* Locate the row r that contains index n.
       We search until (r+1)(r+2)/2 > n, then r is the desired row. */
    idx_t r = 0u;
    while (((r + 1u) * (r + 2u)) / 2u <= n)
    {
        ++r;
    }

    /* Index of the first element of row r. */
    idx_t prev_total = (r * (r + 1u)) / 2u;
    idx_t k = n - prev_total;          /* column within the row (0‑based) */

    /* The first element of any row beyond row 0 is 0. */
    if (k == 0u)
    {
        return (seq_t)0;
    }

    /* Compute the Fuss‑Catalan entry for k > 0. */
    seq_t bin = binomial(r + k - 1u, k - 1u);
    seq_t tmp = bin * (seq_t)(r - k + 2u);
    return tmp / (seq_t)(r + 1u);
}
