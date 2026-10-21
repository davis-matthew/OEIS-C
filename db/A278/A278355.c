/*
 * A278355  a(n) = sum of the perimeters of the Ferrers boards of the
 *          partitions of n. Also, sum of the perimeters of the diagrams of
 *          the regions of the set of partitions of n.
 * https://oeis.org/A278355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * Sum_{k=0}^{n-1} p(k) * d(n-k)
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p(k) is the k‑th partition number and d(m) is the divisor count of m. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Compute partition numbers p[0..n-1] using Euler's pentagonal recurrence. */
    seq_t *p = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!p) return 0;                 /* safety, should not happen in tests */

    p[0] = (seq_t)1;
    for (idx_t m = 1; m < n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            if (k & 1)
                sum += p[m - g1];
            else
                sum -= p[m - g1];
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= m) {
                if (k & 1)
                    sum += p[m - g2];
                else
                    sum -= p[m - g2];
            }
        }
        p[m] = sum;
    }

    /* Compute divisor counts d[1..n] by a simple sieve.  d[0] is left as 0. */
    seq_t *d = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!d) { free(p); return 0; }
    for (idx_t i = 1; i <= n; ++i) d[i] = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i)
            d[j] += (seq_t)1;
    }

    /* Sum over k=0..n-1 : p(k) * d(n-k)  (d[0] is 0). */
    seq_t sum_parts = 0;
    for (idx_t k = 0; k < n; ++k)
        sum_parts += p[k] * d[n - k];

    seq_t result = sum_parts * (seq_t)4;

    free(p);
    free(d);
    return result;
}
