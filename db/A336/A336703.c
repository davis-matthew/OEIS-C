/*
 * A336703  Rectangular array read by antidiagonals. T(n,k) is the number of
 *          length k walks from {} to [n] in the digraph representation of the
 *          superset/subset relation on P([n]) the powerset of [n], n>=0,
 *          k>=0.
 * https://oeis.org/A336703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of length k walks from the empty set ∅ to the full
 *            set [i] in the digraph where vertices are subsets of [i] and
 *            there is a directed edge between two vertices whenever one is a
 *            subset of the other. The pair (i,k) is obtained from the index n
 *            by reading the rectangle of T(i,k) values...
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation details:
    - The index n is partitioned into u and v with u+v=s where
      s=floor((sqrt(8*n+1)-1)/2).  The offset offset = n - s*(s+1)/2
      gives u=offset, v=s-offset.
    - T(u,v) is computed by dynamic programming on subset sizes.
      dp_t[t] = number of length step walks that end in a subset of size t.
      Transition: from size t, one can go to size r in one step
          * if r==t          : 1 possibility (self‑loop)
          * if r>t           : C(u-t, r-t) possibilities (a superset)
          * if r<t           : C(t, r)     possibilities (a subset)
    - Binomial coefficients for 0≤x≤u are pre‑computed with Pascal’s rule.
      All arithmetic uses the type seq_t; for builds where seq_t is a
      64‑bit unsigned integer the algorithm stays within bounds for all
      indices where the final result also fits in 64 bits.  For arbitrary
      precision builds the big‑integer class handles large values automatically.
*/

#include <stdint.h>
#include <math.h>
#include <stdlib.h>

static seq_t compute_T(idx_t N, idx_t K)
{
    /* N – row index (size of the ground set), K – walk length */
    if (K == 0) return (N == 0) ? 1 : 0;

    size_t n = (size_t)N;
    size_t k = (size_t)K;
    size_t dim = n + 1;

    /* binomial coefficients C[x][y] for 0≤x≤n, 0≤y≤x */
    seq_t *C = (seq_t *)malloc(dim * dim * sizeof(seq_t));
    for (size_t i = 0; i < dim * dim; ++i) C[i] = 0;
    C[0 * dim + 0] = 1;
    for (size_t i = 1; i < dim; ++i) {
        C[i * dim + 0] = 1;
        C[i * dim + i] = 1;
        for (size_t j = 1; j < i; ++j)
            C[i * dim + j] = C[(i - 1) * dim + (j - 1)] + C[(i - 1) * dim + j];
    }

    /* dynamic programming arrays */
    seq_t *cur = (seq_t *)calloc(dim, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(dim, sizeof(seq_t));
    cur[0] = 1;   /* start at the empty set */

    for (size_t step = 0; step < k; ++step) {
        for (size_t idx = 0; idx < dim; ++idx) nxt[idx] = 0;
        for (size_t t = 0; t < dim; ++t) {
            seq_t ways = cur[t];
            if (!ways) continue;

            /* stay in the same subset */
            nxt[t] += ways;

            /* move to a larger subset (superset) */
            for (size_t r = t + 1; r < dim; ++r) {
                seq_t trans = C[(n - t) * dim + (r - t)];
                nxt[r] += ways * trans;
            }

            /* move to a smaller subset (subset) */
            for (size_t r = 0; r < t; ++r) {
                seq_t trans = C[t * dim + r];
                nxt[r] += ways * trans;
            }
        }
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    seq_t result = cur[n];
    free(cur);
    free(nxt);
    free(C);
    return result;
}

seq_t a(idx_t n)
{
    /* Determine the antidiagonal coordinates i, j from the index n */
    idx_t s = (idx_t)((sqrt((long double)8.0 * n + 1.0) - 1.0) / 2.0);
    idx_t offset = n - s * (s + 1) / 2;
    idx_t i = offset;
    idx_t j = s - offset;
    return compute_T(i, j);
}
