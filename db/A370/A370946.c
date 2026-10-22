/*
 * A370946  Number of partitions of [n] whose non-singleton elements sum to n.
 * https://oeis.org/A370946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of [n] whose non-singleton elements
 *            sum to n
 * Range:     u8 n <= 20, u16 n <= 43, u32 n <= 103, u64 n <= 268,
 *            u128 n <= 746, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the required counts via combinatorial identities:
   - f(n,m) = number of subsets of [n] with size m and sum n.
     This equals q_distinct(n,m) = number of partitions of n into m distinct parts.
     By a standard bijection, q_distinct(n,m) = p(n - m(m-1)/2, m),
     where p(N,k) is the number of partitions of N into exactly k positive integers.
   - B(m) = number of set partitions of m labeled elements into blocks of size ≥ 2.
     B(0)=1; for m≥1,
       B(m) = Σ_{s=2..m} C(m-1, s-1) * B(m-s).
   Then a(n) = Σ_{m≥1} f(n,m) * B(m).  */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;               /* a(0) = 1 */

    /* Determine maximal block size needed */
    idx_t max_m = 0;
    while ((max_m + 1) * (max_m + 2) / 2 <= n) ++max_m;

    /* DP table for p(N,k): partitions of N into exactly k parts */
    size_t rows = (size_t)n + 1;
    size_t cols = (size_t)max_m + 1;
    seq_t *p = (seq_t *)calloc(rows * cols, sizeof(seq_t));
    if (!p) return 0;                    /* memory allocation failure */

    p[0 * cols + 0] = 1;                 /* p(0,0)=1 */

    for (idx_t N = 1; N <= n; ++N) {
        for (idx_t m = 1; m <= max_m && m <= N; ++m) {
            seq_t val = p[(N - 1) * cols + (m - 1)];
            if (N >= m) val += p[(N - m) * cols + m];
            p[N * cols + m] = val;
        }
    }

    /* Compute B(m): partitions into blocks of size ≥ 2 */
    seq_t *B = (seq_t *)calloc((size_t)max_m + 1, sizeof(seq_t));
    if (!B) { free(p); return 0; }
    B[0] = 1;
    for (idx_t m = 1; m <= max_m; ++m) {
        seq_t sum = 0;
        for (idx_t s = 2; s <= m; ++s) {
            seq_t bin = binom(m - 1, s - 1);
            seq_t sub = B[m - s];
            sum += bin * sub;
        }
        B[m] = sum;
    }

    /* Compute final result */
    seq_t result = 0;
    for (idx_t m = 1; m <= max_m; ++m) {
        idx_t Nprime = n - m * (m - 1) / 2;
        if (Nprime < m) continue;        /* no partitions of Nprime into m parts */
        seq_t f = p[Nprime * cols + m];
        result += f * B[m];
    }

    free(p);
    free(B);
    return result;
}
