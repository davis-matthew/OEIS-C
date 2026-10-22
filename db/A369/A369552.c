/*
 * A369552  Expansion of g.f. A(x) satisfying A(x) = A( x^2*(1+x)^2 ) / x.
 * https://oeis.org/A369552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = A(x^2*(1+x)^2) / x Recurrence: a(n) = Σ_{k=1}^{⌊(n+1)/2⌋}
 *            a(k) * C(2k, n+1-2k), with a(1) = 1. Here C(2k, m) is the
 *            binomial coefficient
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 60, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, return 0 */

    /* Allocate array a[0..n] (index 0 unused). */
    seq_t *a_seq = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!a_seq) return 0;          /* allocation failed */

    /* Initial term */
    a_seq[1] = 1;

    /* We only need k up to floor((n+1)/2); larger k contribute only to indices > n. */
    idx_t max_k = (n > 1) ? (n + 1) / 2 : 1;

    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t comb = 1;                 /* C(2k, 0) */
        idx_t limit_t = 2 * k;          /* last binomial index */

        for (idx_t t = 0; t <= limit_t; ++t) {
            /* Index n_val where this term contributes: n_val = t + 2k - 1 */
            size_t n_val = (size_t)(t + 2 * k - 1);

            if (n_val <= n && n_val > k) {  /* skip n_val==k to avoid double‑counting a(1) */
                a_seq[n_val] += a_seq[k] * comb;
            }

            /* Update combinatorial coefficient for next t */
            if (t < limit_t) {
                comb = (comb * (seq_t)(2 * k - t)) / (seq_t)(t + 1);
            }
        }
    }

    seq_t result = a_seq[n];
    free(a_seq);
    return result;
}
