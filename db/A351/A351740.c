/*
 * A351740  Total number of parts in all partitions of n into parts of size 1,
 *          5, 10 or 25.
 * https://oeis.org/A351740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of parts in all partitions of n into parts
 *            {1,5,10,25}
 * Range:     u8 n <= 31, u16 n <= 177, u32 n <= 3112, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed using dynamic programming with two arrays: P[m] = number of partitions of m, Q[m] = total parts in all such partitions. */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0; /* trivial case */

#ifdef __cplusplus
    std::vector<seq_t> P(n + 1, 0);
    std::vector<seq_t> Q(n + 1, 0);
    P[0] = 1;
    const idx_t parts[4] = {1, 5, 10, 25};
    for (int pi = 0; pi < 4; ++pi) {
        idx_t part = parts[pi];
        for (idx_t m = part; m <= n; ++m) {
            seq_t p = P[m - part];
            seq_t q = Q[m - part];
            P[m] += p;
            Q[m] += q + p;
        }
    }
    return Q[n];
#else
    size_t N = (size_t)n;
    seq_t *P = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *Q = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!P || !Q) {
        free(P);
        free(Q);
        return 0; /* allocation failure unlikely in tests */
    }
    P[0] = 1; /* empty partition */
    const idx_t parts[4] = {1, 5, 10, 25};
    for (int pi = 0; pi < 4; ++pi) {
        idx_t part = parts[pi];
        for (idx_t m = part; m <= n; ++m) {
            seq_t p = P[m - part];
            seq_t q = Q[m - part];
            P[m] += p;
            Q[m] += q + p;
        }
    }
    seq_t result = Q[n];
    free(P);
    free(Q);
    return result;
#endif
}
