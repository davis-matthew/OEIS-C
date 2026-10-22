/*
 * A304788  Expansion of e.g.f. exp(Sum_{k>=1} binomial(2*k,k)*x^k/(k + 1)!).
 * https://oeis.org/A304788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1; 
 * for n > 0, a(n) = Σ_{j=0}^{n-1} binom(n-1, j) * a(j) * Catalan(n-j),
 * where Catalan(k) = binomial(2k, k)/(k+1)
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* assumed definitions in the build */
typedef uint64_t idx_t;

/* seq_t and helper functions are provided externally by the build */

seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *a_cache = NULL;
    static seq_t *cat_cache = NULL;
    static size_t cache_cap = 0;

    if (cache_cap == 0) {
        /* initialise cache for 0 */
        cache_cap = 1;
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        cat_cache = (seq_t*)malloc(sizeof(seq_t));
        a_cache[0] = 1;
        cat_cache[0] = 1;   /* Catalan(0) = 1, not used but convenient */
    }

    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        /* reallocate */
        a_cache  = (seq_t*)realloc(a_cache,  new_cap * sizeof(seq_t));
        cat_cache = (seq_t*)realloc(cat_cache, new_cap * sizeof(seq_t));

        /* compute new entries */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            /* Catalan(i) */
            if (i == 0) {
                cat_cache[0] = 1;
            } else {
                /* cat[i] = cat[i-1] * 2*(2*(i-1)+1) / (i+1) */
                seq_t tmp = cat_cache[i-1];
                tmp = tmp * ( (seq_t)(2 * (2*(i-1) + 1)) );
                tmp = tmp / ( (seq_t)(i + 1) );
                cat_cache[i] = tmp;
            }

            /* compute a[i] using recurrence */
            if (i == 0) {
                a_cache[0] = 1;
            } else {
                seq_t sum = 0;
                seq_t binom = 1;      /* binom(i-1,0) */
                for (idx_t j = 0; j < i; ++j) {
                    seq_t term = a_cache[j] * cat_cache[i - j] * binom;
                    sum = sum + term;
                    if (j + 1 < i) {
                        seq_t factor1 = (seq_t)(i - 1 - j);
                        seq_t factor2 = (seq_t)(j + 1);
                        binom = binom * factor1 / factor2;
                    }
                }
                a_cache[i] = sum;
            }
        }
        cache_cap = new_cap;
    }

    return a_cache[n];
}
