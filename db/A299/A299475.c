/*
 * A299475  a(n) is the number of vertices in the diagram of partitions of n
 *          (see example).
 * https://oeis.org/A299475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 3*A000041(n), a(0) = 1
 * Range:     u8 n <= 12, u16 n <= 37, u32 n <= 117, u64 n <= 398,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is defined as 1. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate array for partition numbers p[0..n]. */
    size_t sz = (size_t)n + 1U;
    seq_t *p = (seq_t *)malloc(sz * sizeof(seq_t));
    if (p == NULL)
        return (seq_t)0;            /* allocation failure (should not happen) */

    p[0] = (seq_t)1;                /* p(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t plus  = (seq_t)0;
        seq_t minus = (seq_t)0;
        for (int k = 1; ; ++k) {
            idx_t g1 = (idx_t)k * (((idx_t)3 * k) - 1) / 2;
            if (g1 > i)
                break;
            if (k & 1)
                plus += p[i - g1];
            else
                minus += p[i - g1];

            idx_t g2 = (idx_t)k * (((idx_t)3 * k) + 1) / 2;
            if (g2 > i)
                continue;
            if (k & 1)
                plus += p[i - g2];
            else
                minus += p[i - g2];
        }
        /* p(i) = plus - minus; plus >= minus for i >= 1. */
        p[i] = plus - minus;
    }

    /* a(n) = 1 + 3 * p(n). */
    seq_t result = (seq_t)1 + (seq_t)3 * p[(size_t)n];

    free(p);
    return result;
}
