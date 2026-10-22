/*
 * A341253  Expansion of (-1 + Product_{k>=1} 1 / (1 + (-x)^k))^10.
 * https://oeis.org/A341253
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in (∑_{k≥0} p_odd(k) x^k)^10,
 *            where p_odd(k) is the number of partitions of k into distinct
 *            odd parts. The generating function of p_odd is ∏_{k≥1}(1 +
 *            x^{2k-1})
 * Range:     u8 n <= 15, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed coefficients */
static idx_t  cached_n = 0;   /* largest n for which coefficients are stored */
static seq_t *coeff_arr = NULL;   /* array of coefficients 0..cached_n */

/* Recompute all coefficients up to N (N ≥ cached_n). */
static void recompute_upto(idx_t N)
{
    if (N < cached_n) return;   /* already have enough */

    /* Allocate temporary arrays, size N+1. */
    size_t sz = (size_t)(N + 1);
    seq_t *p      = (seq_t *)calloc(sz, sizeof(seq_t));   /* p_odd counts   */
    seq_t *prev   = (seq_t *)calloc(sz, sizeof(seq_t));   /* convolution   */
    seq_t *next   = (seq_t *)calloc(sz, sizeof(seq_t));

    if (p == NULL || prev == NULL || next == NULL) {
        /* In the unlikely event of allocation failure, just return
           with no update; behaviour below the offset is undefined. */
        return;
    }

    /* Build p_odd(k) by DP over odd parts. */
    p[0] = 1;   /* empty partition */
    for (idx_t k = 1; k <= N; k += 2) {          /* odd parts only */
        for (idx_t i = N; i >= k; --i) {
            p[i] += p[i - k];
        }
    }

    /* Convolution: raise the polynomial ∑ p[k]x^k to the 10th power. */
    prev[0] = 1;   /* (∑ p[k]x^k)^0 = 1 */
    for (int t = 1; t <= 10; ++t) {
        /* reset next array */
        for (idx_t i = 0; i <= N; ++i) next[i] = 0;

        for (idx_t k = 1; k <= N; ++k) {
            seq_t pk = p[k];
            if (!pk) continue;
            for (idx_t i = k; i <= N; ++i) {
                next[i] += pk * prev[i - k];
            }
        }
        /* swap arrays for next iteration */
        seq_t *tmp  = prev;
        prev         = next;
        next         = tmp;
    }

    /* Allocate new coefficient array, copy results into it. */
    seq_t *new_coeff = (seq_t *)malloc(sz * sizeof(seq_t));
    if (new_coeff != NULL) {
        for (idx_t i = 0; i <= N; ++i) new_coeff[i] = prev[i];
    }

    /* Clean up old cache and temporaries. */
    if (coeff_arr) free(coeff_arr);
    coeff_arr = new_coeff;
    cached_n  = N;

    free(p);
    free(prev);
    free(next);
}

seq_t a(idx_t n)
{
    if (n < 10) return 0;   /* below the official offset */

    if (n > cached_n) {
        /* Recompute up to requested n. */
        recompute_upto(n);
    }

    return coeff_arr ? coeff_arr[n] : 0;
}
