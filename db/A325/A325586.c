/*
 * A325586  G.f.: Sum_{n>=0} (n+1)*(n+2)/2 * x^n * (1+x)^(n*(n+2)).
 * https://oeis.org/A325586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} ((k+1)*(k+2)/2) * binomial(k*(k+2), n-k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 28,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

/* cache of already computed values */
static seq_t *cache = NULL;
static idx_t  computed_max = 0;

/* recompute all values up to N (inclusive) */
static void compute_upto(idx_t N)
{
    const size_t size = (size_t)(N + 1);
    seq_t *new_cache = seq_alloc_new_array(size);

    /* initialise to zero */
    for (size_t i = 0; i < size; ++i)
        new_cache[i] = 0;

    for (idx_t k = 0; k <= N; ++k) {
        idx_t t     = k * (k + 2);      /* exponent of (1+x) */
        seq_t coeff = (seq_t)(k + 1) * (seq_t)(k + 2);
        coeff /= (seq_t)2;              /* coefficient (k+1)(k+2)/2 */

        seq_t bin = 1;                  /* binom(t,0) */

        /* m = 0 contributes to a[k] */
        new_cache[k] += coeff * bin;

        /* remaining m = 1 .. min(t, N-k) */
        idx_t max_m = t;
        if (max_m > N - k)
            max_m = N - k;

        for (idx_t m = 1; m <= max_m; ++m) {
            bin *= (seq_t)(t - m + 1);
            bin /= (seq_t)m;          /* binom(t,m) */

            idx_t idx = k + m;        /* position n = k + m */
            new_cache[idx] += coeff * bin;
        }
    }

    /* replace old cache */
    if (cache != NULL)
        free_seq_array(cache);
    cache = new_cache;
    computed_max = N;
}

/* external sequence function */
extern seq_t a(idx_t n)
{
    if (cache == NULL || n > computed_max)
        compute_upto(n);
    return cache[n];
}
