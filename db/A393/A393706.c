/*
 * A393706  Square array A(n, k), n, k > 0, read by upwards antidiagonals; the
 *          n-th row lists the nonnegative numbers m such that A000217(m) is a
 *          multiple of n.
 * https://oeis.org/A393706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value at the n-th position in the square array
 *            A(n,k) * read by upwards antidiagonals. For each n we compute
 *            the corresponding * diagonal and position, then find the k-th
 *            integer m in row r such that * the triangular number
 *            T(m)=m(m+1)/2 is a multiple of r
 * Range:     u8 n <= 511, u16 n <= 9491, u32 n <= 9491, u64 n <= 9491,
 *            u128 n <= 9491, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the diagonal d containing term n.
     * All terms up to diagonal d inclusive are (d-1)*d/2 in total.
     */
    idx_t d = 2;
    while ((d - 1) * d / 2 < n)
        ++d;

    /* Number of terms before this diagonal */
    idx_t prev = (d - 2) * (d - 1) / 2;
    /* Position within the diagonal (1‑based) */
    idx_t pos = n - prev;
    /* Corresponding row and column */
    idx_t row = d - pos;
    idx_t col = pos;          /* 1‑based index in the row */

    /* Compute the col‑th element of row 'row' */
    seq_t L = (seq_t)(2 * row);            /* period of residues */
    seq_t residues[300];                   /* max L is < 300 for all rows that can appear */
    size_t count = 0;

    /* Find all residues r in [0, L-1] for which T(r) is a multiple of 'row' */
    for (seq_t m = 0; m < L; ++m) {
        seq_t prod = (m * (m + 1)) / 2;    /* triangular number of m */
        if (prod % row == 0)
            residues[count++] = m;
    }

    /* k-th element (k = col) is the t‑th in the ordered union of
     * arithmetic progressions with common difference L:
     *   value = floor((k-1)/count) * L + residues[(k-1)%count]
     */
    size_t t = (size_t)(col - 1);
    size_t t_div = t / count;
    size_t t_mod = t % count;

    return (seq_t)t_div * L + residues[t_mod];
}
