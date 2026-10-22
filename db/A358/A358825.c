/*
 * A358825  Number of ways to choose a sequence of integer partitions, one of
 *          each part of an integer partition of n into odd parts.
 * https://oeis.org/A358825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{odd k <= n} p(k) * a(n-k) where a(0) = 1 and p(k) is
 *            the partition number of k. The algorithm first computes p(k) for
 *            all k <= n, then uses the recurrence to obtain a(n)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate arrays of size n+1 */
    size_t N = (size_t)(n + 1);
    seq_t* p = (seq_t*)malloc(N * sizeof(seq_t));
    seq_t* A = (seq_t*)malloc(N * sizeof(seq_t));
    if (!p || !A) return 0;          /* out of memory */

    /* initialize */
    for (idx_t i = 0; i <= n; ++i) { p[i] = 0; A[i] = 0; }
    p[0] = 1;
    A[0] = 1;

    /* compute partition numbers p(k) */
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t j = k; j <= n; ++j) {
            p[j] += p[j - k];
        }
    }

    /* compute the sequence a(n) */
    for (idx_t k = 1; k <= n; k += 2) {     /* odd parts only */
        seq_t pk = p[k];
        if (pk == 0) continue;
        for (idx_t m = k; m <= n; ++m) {
            A[m] += pk * A[m - k];
        }
    }

    seq_t result = A[n];

    free(p);
    free(A);
    return result;
}
