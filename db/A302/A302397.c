/*
 * A302397  Expansion of e.g.f. 1/(1 + x*exp(x)).
 * https://oeis.org/A302397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -n * Σ_{k=0}^{n-1} a(k) * binomial(n-1, k)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 23, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *seq_cache = NULL;   /* cache of computed a(k) values */
static size_t cache_cap = 0;      /* allocated size of seq_cache */
static idx_t cache_max = 0;       /* highest index computed so far */

static void ensure_cache(idx_t n)
{
    if (cache_cap >= (size_t)(n + 1)) return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return; /* out of memory */
    if (seq_cache) {
        /* copy all computed entries, including cache_max */
        memcpy(new_cache, seq_cache, (cache_max + 1) * sizeof(seq_t));
        free(seq_cache);
    }
    seq_cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Base case: a(0) = 1 */
    if (n == 0) {
        if (!seq_cache) {
            ensure_cache(0);
            seq_cache[0] = (seq_t)1;
            cache_max = 0;
        }
        return seq_cache[0];
    }

    /* Extend cache if necessary */
    if (n > cache_max) {
        ensure_cache(n);
        /* Compute values up to n lazily */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            idx_t below = i - 1;
            seq_t sum = 0;
            seq_t binom = 1; /* C(below,0) */
            for (idx_t k = 0; k < i; ++k) {
                seq_t term = seq_cache[k] * binom;
                sum += term;
                if (k + 1 < i) {
                    /* binom = binom * (below - k) / (k + 1) */
                    binom = (binom * (seq_t)(below - k)) / (seq_t)(k + 1);
                }
            }
            seq_cache[i] = - (seq_t)i * sum;
        }
        cache_max = n;
    }
    return seq_cache[n];
}
