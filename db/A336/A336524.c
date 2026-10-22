/*
 * A336524  Triangular array read by rows. T(n,k) is the number of unlabeled
 *          binary trees with n internal nodes and exactly k distinguished
 *          external nodes (leaves) for 0 <= k <= n+1 and n >= 0.
 * https://oeis.org/A336524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened row‑wise enumeration of the triangle *
 *            T(n,k) = binomial(n+k,n)*binomial(2*n+1,n+k)/(2*n+1), * where k
 *            ranges from 0 to n+1. * The sequence a(n) lists the values row
 *            by row
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find the row i such that the n-th element lies in row i.  */
    idx_t i = 0;
    while (((i + 1) * (i + 4)) / 2 <= n) {
        ++i;
    }

    /* Number of elements before row i. */
    idx_t prev = i * (i + 3) / 2;
    /* Column index k within the row. */
    idx_t k = n - prev;

    /* Compute the binomial terms. */
    seq_t c1 = binomial(i + k, i);
    seq_t c2 = binomial(2 * i + 1, i + k);
    seq_t denom = (seq_t)(2 * i + 1);

    return (c1 * c2) / denom;
}
