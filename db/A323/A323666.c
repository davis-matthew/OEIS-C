/*
 * A323666  Expansion of e.g.f. exp(exp(2*x)*BesselI(0,2*x) - 1).
 * https://oeis.org/A323666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} C(2k,k) * C(n-1,k-1) * a(n-k) Initial value:
 *            a(0) = 1 This implementation uses memoisation and memoises the
 *            central binomial coefficients C(2k,k) to achieve O(n²) time and
 *            small memory usage
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;        /* cache of a(i) values */
static size_t cache_cap = 0;       /* current cache size */

static seq_t *binom2k = NULL;      /* binom(2k,k) for k up to current max */
static size_t binom2k_cap = 0;     /* current capacity of binom2k */

seq_t a(idx_t n)
{
    /* initialise cache[0] */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        assert(cache != NULL);
        cache[0] = (seq_t)1;
        cache_cap = 1;
    }

    /* enlarge cache if necessary and compute missing values */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        assert(cache != NULL);

        /* Ensure binom(2k,k) up to k = n is available */
        if (binom2k == NULL) {
            binom2k = (seq_t*)malloc((n + 1) * sizeof(seq_t));
            assert(binom2k != NULL);
            binom2k[0] = (seq_t)1;          /* C(0,0) = 1 */
            binom2k_cap = 1;
        }
        if ((size_t)n + 1 > binom2k_cap) {
            size_t new_cap_k = (size_t)n + 1;
            binom2k = (seq_t*)realloc(binom2k, new_cap_k * sizeof(seq_t));
            assert(binom2k != NULL);
            for (idx_t k = (idx_t)binom2k_cap; k <= n; ++k) {
                seq_t prev = binom2k[k-1];
                seq_t num = (seq_t)(2*k) * (seq_t)(2*k - 1);
                seq_t den = (seq_t)k * (seq_t)k;
                binom2k[k] = prev * num / den;   /* exact division */
            }
            binom2k_cap = new_cap_k;
        }

        /* Compute a(i) for each new index i */
        for (idx_t i = (idx_t)cache_cap; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            seq_t c2 = (seq_t)1;   /* binom(i-1,0) */

            for (idx_t k = 1; k <= i; ++k) {
                seq_t c1  = binom2k[k];                /* C(2k,k) */
                seq_t prod = c1 * c2 * cache[i - k];   /* product */
                sum = sum + prod;

                /* update binom(i-1,k) for next k (if needed) */
                if (k < i) {
                    c2 = c2 * (seq_t)(i - k) / (idx_t)k;
                }
            }
            cache[i] = sum;
        }
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
