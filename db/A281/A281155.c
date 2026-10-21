/*
 * A281155  Expansion of (Sum_{k>=2} x^(k^2))^3.
 * https://oeis.org/A281155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples of squares > 1 summing to n.
 *            The implementation computes the coefficient of x^n in
 *            (∑_{k≥2}x^{k^2})^3 by a double‑summing convolution
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* Build the list of squares k^2 with k >= 2 and k^2 ≤ n. */
    size_t est = (size_t)sqrt((double)n) + 2;          /* safety margin   */
    idx_t *sqs = (idx_t*)malloc(est * sizeof(idx_t));
    if (!sqs) return 0;          /* allocation failed – return 0 */
    size_t m = 0;
    for (idx_t k = 2; ; ++k) {
        idx_t sq = k * k;
        if (sq > n) break;
        if (m >= est) {                    /* grow the array if needed */
            est *= 2;
            sqs = (idx_t*)realloc(sqs, est * sizeof(idx_t));
            if (!sqs) return 0;
        }
        sqs[m++] = sq;
    }

    /* conv2[s] = #ordered pairs of squares summing to s */
    seq_t *conv2 = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!conv2) { free(sqs); return 0; }

    for (size_t i = 0; i < m; ++i) {
        idx_t s1 = sqs[i];
        for (size_t j = 0; j < m; ++j) {
            idx_t s2 = sqs[j];
            idx_t sum = s1 + s2;
            if (sum <= n) {
                conv2[sum] += 1;
            }
        }
    }

    /* a[n] = sum_{s in sqs} conv2[n - s] */
    seq_t result = 0;
    for (size_t i = 0; i < m; ++i) {
        idx_t s = sqs[i];
        if (s <= n) {
            result += conv2[n - s];
        }
    }

    free(conv2);
    free(sqs);
    return result;
}
