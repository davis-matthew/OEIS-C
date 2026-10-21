/*
 * A278644  Number of partitions of n into parts of sorts {1, 2, ... }.
 * https://oeis.org/A278644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} p(n,k) * A000670(k) where p(n,k) is the
 *            number of partitions of n into exactly k parts, and A000670(k)
 *            is the k-th Fubini number (number of preferential arrangements
 *            of a set of k elements). The algorithm computes a(n) by
 *            iterating over k and maintaining the array Q(n) that co...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;   /* a(0) = 1 */

    size_t N = (size_t)n;              /* upper bound for loops */
    /* arrays for partitions into at most k parts and Fubini numbers */
    seq_t *Q = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *f = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!Q || !f) {
        /* memory failure: return 0 as a fallback (main program may handle) */
        if (Q) free(Q);
        if (f) free(f);
        return (seq_t)0;
    }

    Q[0] = (seq_t)1;   /* partitions of 0 into 0 parts */
    f[0] = (seq_t)1;   /* Fubini(0) = 1 */

    seq_t ans = (seq_t)0;

    for (size_t k = 1; k <= N; ++k) {
        /* ----- compute Fubini number f[k] ----- */
        seq_t fk = (seq_t)0;
        seq_t binom = (seq_t)1;            /* C(k,0) */
        for (size_t i = 0; i < k; ++i) {
            fk += binom * f[i];
            /* update binom to C(k, i+1) */
            binom = (binom * (seq_t)(k - i)) / (seq_t)(i + 1);
        }
        f[k] = fk;

        /* ----- update partition counts Q[0..N] for at most k parts ----- */
        seq_t prev = Q[N];                 /* value before this k */
        for (size_t j = k; j <= N; ++j) {
            Q[j] += Q[j - k];
        }
        seq_t new_val = Q[N];
        seq_t diff = new_val - prev;       /* p(N, k) = diff */

        /* accumulate contribution to a(N) */
        ans += diff * f[k];
    }

    free(Q);
    free(f);
    return ans;
}
