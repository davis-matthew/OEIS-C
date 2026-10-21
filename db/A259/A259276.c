/*
 * A259276  G.f.: A(x) = exp( Sum_{n>=1} 6^n * x^n/(n*(1+x^n)) ).
 * https://oeis.org/A259276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exp( Sum_{k>=1} 6^k * x^k/(k*(1+x^k)) ) generating a(n)
 *            (OEIS A259276). The implementation uses the recurrence a(0) = 1
 *            n*a(n) = ∑_{k=1}^{n} α(k) * a(n-k) where α(k) = ∑_{g|k}
 *            (-1)^{g-1} g * 6^{k/g}. All arithmetic is performed in type
 *            seq_t exactly. Values for α(k) are positive, so the su...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    idx_t N = n;
    size_t sz = (size_t)N + 1;

    /* Allocate temporary arrays */
    seq_t *pow6    = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *alpha_pos = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *alpha_neg = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *alpha   = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *a_seq   = (seq_t *)malloc(sz * sizeof(seq_t));

    if (!pow6 || !alpha_pos || !alpha_neg || !alpha || !a_seq) {
        /* Allocation failed – abort (should not happen in test harness) */
        exit(EXIT_FAILURE);
    }

    /* 6^d for d = 0..N */
    pow6[0] = (seq_t)1;
    for (idx_t d = 1; d <= N; ++d) {
        pow6[d] = pow6[d - 1] * (seq_t)6;
    }

    /* Compute α(k) = α_pos(k) – α_neg(k) where
       α_pos(k) = ∑_{g odd, g|k} g * 6^{k/g}
       α_neg(k) = ∑_{g even, g|k} g * 6^{k/g}       */
    for (idx_t g = 1; g <= N; ++g) {
        seq_t g_seq = (seq_t)g;
        for (idx_t d = 1, k = g; k <= N; ++d, k += g) {
            /* term = g * 6^d */
            seq_t term = pow6[d] * g_seq;
            if ((g & 1) == 1) {
                alpha_pos[k] += term;
            } else {
                alpha_neg[k] += term;
            }
        }
    }

    for (idx_t k = 1; k <= N; ++k) {
        /* α(k) = α_pos(k) - α_neg(k); α_pos(k) > α_neg(k) */
        alpha[k] = alpha_pos[k] - alpha_neg[k];
    }

    /* Reuse α_pos and α_neg memory for recurrence to reduce footprint */
    free(alpha_pos);
    free(alpha_neg);
    /* α array becomes local after use; keep for recurrence. */

    /* Compute a(n) using the recurrence */
    a_seq[0] = (seq_t)1;
    for (idx_t i = 1; i <= N; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += alpha[k] * a_seq[i - k];
        }
        a_seq[i] = sum / i;
    }

    seq_t result = a_seq[N];

    /* Free allocated memory */
    free(pow6);
    free(alpha);
    free(a_seq);

    return result;
}
