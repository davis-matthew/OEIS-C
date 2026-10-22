/*
 * A328093  Expansion of (theta_3(z)*theta_3(23z) +
 *          theta_2(z)*theta_2(23z))^5.
 * https://oeis.org/A328093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of qⁿ in (θ₃(z)·θ₃(23z) + θ₂(z)·θ₂(23z))⁵
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 401, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The integers idx_t and seq_t are supplied by the build. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Number of terms to compute (indices 0..n). */
    idx_t N = n;
    size_t sz = (size_t)(N + 1);

    /* Allocate coefficient array S for S(q) = θ₃*θ₃ + θ₂*θ₂. */
    seq_t *S = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!S) return (seq_t)0;

    /* Contributions from θ₃(z)*θ₃(23z): Σ q^{m²+23l²}.  */
    for (idx_t m = 0; m * m <= N; ++m) {
        idx_t m2 = m * m;
        for (idx_t l = 0; 23 * l * l <= N - m2; ++l) {
            idx_t k = m2 + 23 * l * l;
            if (k > N) continue;
            seq_t w = (m == 0 ? 1 : 2) * (l == 0 ? 1 : 2);
            S[(size_t)k] += w;
        }
    }

    /* Contributions from θ₂(z)*θ₂(23z):
       exponent = m² + 23l² + m + 23l + 6.  Each (m,l)≥0 contributes 4 terms
       due to symmetry of ±m‑1 and ±l‑1. */
    for (idx_t m = 0; m * m <= N; ++m) {
        idx_t m2 = m * m;
        for (idx_t l = 0; 23 * l * l <= N - m2; ++l) {
            idx_t k = m2 + 23 * l * l + m + 23 * l + 6;
            if (k > N) break;          /* further l will give larger k */
            S[(size_t)k] += 4;
        }
    }

    /* Allocate working arrays for convolution. */
    seq_t *cur  = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!cur || !next) {
        free(S);
        free(cur);
        free(next);
        return (seq_t)0;
    }

    /* cur = S^1 initially. */
    for (size_t i = 0; i < sz; ++i) cur[i] = S[i];

    /* Multiply cur by S four more times to obtain S^5. */
    for (int step = 0; step < 4; ++step) {
        /* Zero next array. */
        memset((void *)next, 0, sz * sizeof(seq_t));

        for (size_t k = 0; k < sz; ++k) {
            if (cur[k] == 0) continue;
            seq_t ck = cur[k];
            for (size_t i = 0; i + k < sz; ++i) {
                if (S[i] == 0) continue;
                next[i + k] += ck * S[i];
            }
        }
        /* Swap cur and next for the next iteration. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t result = cur[(size_t)N];

    /* Free all allocated memory. */
    free(S);
    free(cur);
    free(next);

    return result;
}
