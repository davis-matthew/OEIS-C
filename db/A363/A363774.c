/*
 * A363774  Expansion of 1/(Sum_{k>=0} x^(k^2))^2.
 * https://oeis.org/A363774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 17, i16 n <= 39, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A363774
   Expansion of 1/(∑_{k≥0} x^{k²})²
   a(0) = 1
   a(n) = -(2/n) * Σ_{k=1..n} A162552(k) * a(n-k)   (equivalent recurrence)
   Recurrence used in code: a(0)=1, for n≥1
   a(n) = - Σ_{k=0}^{n-1} h[n-k] * a(k)
   where h(m) = number of ordered pairs (i,j≥0) with i² + j² = m.
*/
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static storage for previously computed terms */
    static seq_t *a_seq = NULL;   /* a(n) values */
    static size_t a_size = 0;     /* current allocated size (>= n+1) */

    static seq_t *h = NULL;       /* h(m) values */
    static size_t h_size = 0;     /* current h array size */

    /* Extend h array up to required n */
    if (h_size <= n) {
        /* Reallocate and rebuild h entirely (n ≤ 10000, cheap) */
        free(h);
        h = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= n; ++i) h[i] = 0;

        /* Compute all ordered pairs (i,j) with i² + j² ≤ n */
        idx_t sqmax = 0;
        while ((sqmax + 1) * (sqmax + 1) <= n) ++sqmax;

        for (idx_t i = 0; i <= sqmax; ++i) {
            seq_t i2 = (seq_t)i * i;
            for (idx_t j = 0; j <= sqmax; ++j) {
                seq_t j2 = (seq_t)j * j;
                seq_t m  = i2 + j2;
                if (m > n) break;
                h[(idx_t)m] += 1;
            }
        }
        h_size = n + 1;
    }

    /* Extend a_seq up to required n */
    if (a_size <= n) {
        a_seq = (seq_t *)realloc(a_seq, (n + 1) * sizeof(seq_t));

        /* Initialize first term if not already */
        if (a_size == 0) {
            a_seq[0] = 1;
            a_size = 1;
        }

        /* Compute new terms using recurrence */
        for (idx_t k = a_size; k <= n; ++k) {
            seq_t sum = 0;
            for (idx_t j = 0; j < k; ++j) {
                sum += a_seq[j] * h[k - j];
            }
            a_seq[k] = -sum;
        }
        a_size = n + 1;
    }

    return a_seq[n];
}
