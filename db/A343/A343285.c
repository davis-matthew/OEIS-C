/*
 * A343285  Dirichlet g.f.: Product_{k>=2} 1 / (1 - k^(-s))^(k^5).
 * https://oeis.org/A343285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all factorizations n = ∏ k^e_k * (unordered,
 *            each integer k may appear at most once) of * ∏ binom(k^5 + e_k –
 *            1, e_k) * This follows from the Dirichlet generating function *
 *            ∏_{k≥2} (1 – k^{-s})^{-k^5}. * The following code computes a(n)
 *            using a dynamic‑programming * convolution o...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t base, idx_t exp) */

static seq_t *dp_prev = NULL;   /* array with computed a values   */
static seq_t *dp_next = NULL;   /* temporary array during preprocessing */
static idx_t  max_n   = 0;       /* largest n for which dp_prev is valid */

/* --------------------------------------------------------------------- */
/* Precompute a(n) for 1 ≤ n ≤ up_to and store them in dp_prev          */
static void precompute(idx_t up_to)
{
    /* free any old tables */
    if (dp_prev) {
        free(dp_prev);
        free(dp_next);
    }

    if (up_to < 1) return;   /* nothing to compute */

    size_t sz = (size_t)(up_to + 1);
    dp_prev = (seq_t *)malloc(sz * sizeof(seq_t));
    dp_next = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp_prev || !dp_next) {               /* allocation failure */
        dp_prev = dp_next = NULL;
        return;
    }

    /* initialise: a(1) = 1, all other entries 0 */
    for (idx_t i = 0; i <= up_to; ++i)
        dp_prev[i] = 0;
    dp_prev[1] = 1;

    /* iterate over each base k ≥ 2 */
    for (idx_t k = 2; k <= up_to; ++k) {
        /* copy previous table (case e = 0) */
        for (idx_t i = 1; i <= up_to; ++i)
            dp_next[i] = dp_prev[i];

        seq_t base5 = big_pow((seq_t)k, 5);    /* k^5 in seq_t */
        seq_t w      = base5;                   /* binom(k^5,1) for e=1  */
        idx_t pow_k  = k;                       /* k^e for current e  */
        idx_t e      = 1;                       /* current exponent  */

        while (pow_k <= up_to) {
            /* add contribution of k^e to all multiples of pow_k */
            for (idx_t m = pow_k; m <= up_to; m += pow_k) {
                dp_next[m] += dp_prev[m / pow_k] * w;
            }

            /* prepare for next exponent e+1 */
            ++e;
            if (pow_k > up_to / k) break;           /* overflow guard */
            pow_k *= k;                              
            /* w = binom(k^5 + (e-1), e) */
            w = w * (base5 + (seq_t)(e - 1)) / (seq_t)e;
        }

        /* swap dp arrays for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev   = dp_next;
        dp_next   = tmp;
    }

    max_n = up_to;
}

/* --------------------------------------------------------------------- */
/* Required public function                                             */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                        /* out of bounds */
    if (n > max_n) precompute(n);                /* extend table if needed */
    return dp_prev ? dp_prev[n] : 0;
}
