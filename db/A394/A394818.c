/*
 * A394818  Number of set partitions of [n] such that the elements of the k-th
 *          block can each be k different colors.
 * https://oeis.org/A394818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0..n} i! * G_i(n-i) where G_0(0)=1, G_0(k>0)=0 and
 *            for i≥1, G_i(k) = G_{i-1}(k) + t_i * G_i(k-1) with t_i =
 *            i*(i+1)/2. This follows from (1 - t_i*y) * (1/P_i(y)) =
 *            (1/P_{i-1}(y))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate two arrays of size n+1 */
    seq_t *G   = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *newG = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!G || !newG) return (seq_t)0;   /* allocation failed */

    G[0] = (seq_t)1;            /* G_0(0) = 1 */

    seq_t result = (seq_t)0;
    seq_t fact   = (seq_t)1;    /* 0! */

    for (idx_t i = 0; i <= n; ++i) {
        idx_t k = n - i;                     /* we need G_i(k) with k=n-i */
        seq_t contrib = fact * G[k];
        result += contrib;

        if (i == n) break;                  /* last iteration, no G_{i+1} */

        /* t_{i+1} = (i+1)*(i+2)/2 */
        seq_t t = (seq_t)(i + 1) * ((seq_t)(i + 1) + (seq_t)1) / (seq_t)2;

        /* Build G_{i+1} from G_i using the recurrence */
        newG[0] = G[0];
        for (idx_t j = 1; j <= n; ++j) {
            newG[j] = G[j] + t * newG[j - 1];
        }

        /* Swap arrays for next iteration */
        seq_t *tmp = G;
        G = newG;
        newG = tmp;

        /* Update factorial for next i */
        fact *= (seq_t)(i + 1);
    }

    free(G);
    free(newG);
    return result;
}
