/*
 * A295342  The number of partitions of n in which at least one part is a
 *          multiple of 4.
 * https://oeis.org/A295342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) – A001935(n) – number of partitions of n that
 *            contain at least one part that is a multiple of 4
 * Range:     u8 n <= 19, u16 n <= 45, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    size_t cap = N + 1;

    seq_t *p = (seq_t *)malloc(cap * sizeof(seq_t));
    seq_t *q = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!p || !q) return (seq_t)0;   /* fallback on allocation failure */

    /* initialise arrays */
    for (size_t i = 0; i < cap; ++i) {
        p[i] = (seq_t)0;
        q[i] = (seq_t)0;
    }
    p[0] = (seq_t)1;
    q[0] = (seq_t)1;

    /* compute all partitions p[0..N]  (unrestricted parts) */
    for (size_t k = 1; k <= N; ++k) {
        for (size_t i = k; i <= N; ++i) {
            p[i] += p[i - k];
        }
    }

    /* compute partitions q[0..N] with no part divisible by 4 */
    for (size_t k = 1; k <= N; ++k) {
        if (k % 4 == 0) continue;          /* ignore multiples of 4 */
        for (size_t i = k; i <= N; ++i) {
            q[i] += q[i - k];
        }
    }

    seq_t result = p[N] - q[N];

    free(p);
    free(q);
    return result;
}
