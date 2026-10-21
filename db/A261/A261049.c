/*
 * A261049  Expansion of Product_{k>=1} (1+x^k)^(p(k)), where p(k) is the
 *          partition function.
 * https://oeis.org/A261049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1+x^k)^{p(k)}
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 40, u64 n <= 89, u128 n <= 203,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p(k) is the partition function. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* handle the offset */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* length of arrays */
    const size_t N = (size_t)n;

    /* ---- compute partition numbers p[0..N] ---- */
    seq_t *p = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!p) return (seq_t)0;            /* allocation failure */
    p[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i)
        for (size_t j = i; j <= N; ++j)
            p[j] += p[j - i];

    /* ---- arrays for dynamic programming ---- */
    seq_t *dp   = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *bin  = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp || !next || !bin) {
        free(p); free(dp); free(next); free(bin);
        return (seq_t)0;            /* allocation failure */
    }
    dp[0] = (seq_t)1;                 /* initial polynomial is 1 */

    /* ---- main loop over k ---- */
    for (size_t k = 1; k <= N; ++k) {
        /* maximum power of x that can be reached with this k */
        const size_t m = N / k;                 /* floor(N/k) */

        const seq_t p_k = p[k];

        /* build binomial coefficients C(p_k, t) for t = 0..m */
        bin[0] = (seq_t)1;
        size_t tmax = 0;
        for (size_t t = 1; t <= m; ++t) {
            if (p_k < (seq_t)t) {               /* C(p_k, t) = 0 for t > p_k */
                tmax = t - 1;
                break;
            }
            seq_t prev = bin[t - 1];
            seq_t mult = prev * (p_k - (seq_t)(t - 1));
            bin[t] = mult / (seq_t)t;
            tmax = t;
        }
        if (tmax == 0) tmax = 0;               /* ensure tmax is valid when p_k==0 */

        /* reset next array */
        memset(next, 0, (N + 1) * sizeof(seq_t));

        /* convolution for each residue modulo k */
        for (size_t r = 0; r < k; ++r) {
            const size_t max_j = (N - r) / k;  /* number of terms in this residue */
            for (size_t j = 0; j <= max_j; ++j) {
                seq_t accum = (seq_t)0;
                size_t tlimit = (j < tmax) ? j : tmax;
                for (size_t t = 0; t <= tlimit; ++t) {
                    accum += bin[t] * dp[r + (j - t) * k];
                }
                next[r + j * k] = accum;
            }
        }

        /* prepare for next iteration */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* final result: coefficient of x^N */
    seq_t result = dp[N];

    /* free allocated memory */
    free(dp);
    free(next);
    free(p);
    free(bin);

    return result;
}
