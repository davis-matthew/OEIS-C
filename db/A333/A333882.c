/*
 * A333882  Expansion of e.g.f. exp(Sum_{k>=0} x^(5*k + 1) / (5*k + 1)!).
 * https://oeis.org/A333882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/5)} binomial(n-1,5*k) *
 *            a(n-5*k-1) (n >= 1)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* static cache of computed values a(0)..a(max_computed) */
    static seq_t *cache = NULL;
    static idx_t next_to_compute = 0;   /* first index that has NOT been computed */
    static size_t cache_capacity = 0;   /* allocated array size (in elements) */

    /* first call: allocate cache and initialise the base case */
    if (cache == NULL) {
        cache_capacity = 1;                    /* hold a(0)           */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return 0;                  /* allocation failure */
        cache[0] = (seq_t)1;                   /* a(0) = 1             */
        next_to_compute = 1;                   /* next missing index   */
    }

    /* enlarge cache if n exceeds current capacity */
    if ((size_t)n + 1 > cache_capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                    /* allocation failure */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = next_to_compute; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t limit = (i == 0) ? 0 : (i - 1) / 5;
        for (idx_t k = 0; k <= limit; ++k) {
            idx_t m = 5 * k;                       /* 5k                */
            seq_t bin = binomial(i - 1, m);        /* binomial coefficient */
            seq_t term = bin * cache[i - m - 1];
            sum += term;
        }
        cache[i] = sum;
    }

    next_to_compute = n + 1;    /* all values up to n are now ready */
    return cache[n];
}
