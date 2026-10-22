/*
 * A373005  Array read by ascending antidiagonals: A(n,k) is the maximum
 *          possible cardinality of a set of points of diameter at most k-1 in
 *          {0,1}^n.
 * https://oeis.org/A373005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,j) where (i,j) is the point at index n in the matrix
 *            A(i,j) of maximum cardinality of a set of diameter ≤ j-1 in
 *            {0,1}^i. A(i,j) is defined by: if j%2==1: A = Σ_{t=0}^{(j-1)/2}
 *            binomial(i,t) else: A = binomial(i-1, j/2-1) + Σ_{t=0}^{j/2-1}
 *            binomial(i,t) (with the convention binomial(n,...
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   antidiagonal_idx, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/antidiagonal_idx.h"

seq_t a(idx_t n) {
    /* 1. locate the position (i,j) on the antidiagonal */
    idx_t d = antidiagonal_idx(n);            /* diagonal number */
    idx_t base = (__int128)d * (d + 1) / 2;   /* first index on this diagonal */
    idx_t pos = n - base;                     /* offset along the diagonal */
    idx_t i = d - pos;                        /* dimension */
    idx_t j = pos;                            /* k value */

    /* 2. compute A(i,j) according to the definition */
    if (j == 0) {                     /* diameter < 0 */
        return (i == 0) ? (seq_t)1 : (seq_t)0;
    }

    if (i == 0) {                     /* special handling for dimension 0 */
        if (j % 2 == 1) {
            return (seq_t)1;
        } else {
            /* j even: A(0,j) = 2 if (j/2) is odd, else 0 */
            return (((j / 2) % 2) == 1) ? (seq_t)2 : (seq_t)0;
        }
    }

    if (j % 2 == 1) {                  /* j is odd */
        idx_t t = (j - 1) / 2;
        seq_t sum = 0;
        for (idx_t k = 0; k <= t; ++k) {
            sum += binomial(i, k);
        }
        return sum;
    } else {                           /* j is even, j >= 2 */
        idx_t t = j / 2;
        seq_t add = binomial(i - 1, t - 1);
        seq_t sum = 0;
        for (idx_t k = 0; k < t; ++k) {
            sum += binomial(i, k);
        }
        return add + sum;
    }
}
