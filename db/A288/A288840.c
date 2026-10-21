/*
 * A288840  Coefficients in expansion of E_8/E_6.
 * https://oeis.org/A288840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in (1 + 480 Σ_{k≥1} σ_7(k) q^k) / (1 -
 *            504 Σ_{k≥1} σ_5(k) q^k). Here σ_s(k)=∑_{d|k} d^s
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   The sequence is the q‑expansion of –q·j′(q)/(j(q)-1728), where
    j(q)=q^−1+744+… is the elliptic modular invariant.  The
    computation below uses the modular form identity

          E₈(q)/E₆(q) = (1 + 480 Σ σ₇(k) q^k) / (1 - 504 Σ σ₅(k) q^k)

    and expands it by convolution.  The implementation
    caches all values up to the largest index requested so
    that repeated calls run in O(1) after the first one. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *sigma5 = NULL;
    static seq_t *sigma7 = NULL;
    static seq_t *inv    = NULL;
    static seq_t *ac     = NULL;
    static size_t max_cached = 0;

    size_t N = (size_t)n;          /* index fits in size_t          */
    if (!ac || N > max_cached) {   /* recompute if out‑of‑range     */
        /* free old buffers */
        free(sigma5);
        free(sigma7);
        free(inv);
        free(ac);

        /* allocate new buffers (size N+1 to include index 0) */
        sigma5 = (seq_t *)calloc(N + 1, sizeof(seq_t));
        sigma7 = (seq_t *)calloc(N + 1, sizeof(seq_t));
        inv    = (seq_t *)calloc(N + 1, sizeof(seq_t));
        ac     = (seq_t *)calloc(N + 1, sizeof(seq_t));

        /* compute σ₅(k) and σ₇(k) for k = 1 … N */
        for (size_t d = 1; d <= N; ++d) {
            seq_t d5  = 1;
            seq_t d7  = 1;
            /* d^5 and d^7 */
            for (size_t i = 0; i < 5; ++i) d5 *= (seq_t)d;
            for (size_t i = 0; i < 7; ++i) d7 *= (seq_t)d;
            for (size_t m = d; m <= N; m += d) {
                sigma5[m] += d5;
                sigma7[m] += d7;
            }
        }

        /* compute the inverse series 1/(1 - 504 Σ σ₅) */
        inv[0] = (seq_t)1;
        for (size_t k = 1; k <= N; ++k) {
            seq_t sum = (seq_t)0;
            for (size_t i = 1; i <= k; ++i) {
                sum += sigma5[i] * inv[k - i];
            }
            inv[k] = (seq_t)(504) * sum;   /* 504 Σ σ₅[i] * inv[k-i]     */
        }

        /* compute a(k) for k = 0 … N */
        ac[0] = (seq_t)1;                 /* a(0) = 1                    */
        for (size_t k = 1; k <= N; ++k) {
            /* start with the constant term from the numerator */
            seq_t ak = inv[k];
            /* add convolution of 480·σ₇ with the inverse series */
            for (size_t i = 1; i <= k; ++i) {
                seq_t term = sigma7[i] * inv[k - i];
                term *= (seq_t)480;
                ak += term;
            }
            ac[k] = ak;
        }

        /* remember the largest computed index */
        max_cached = N;
    }

    return ac[N];
}
