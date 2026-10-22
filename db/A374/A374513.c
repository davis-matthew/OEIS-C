/*
 * A374513  Expansion of 1/(1 - 4*x - 4*x^2)^(7/2).
 * https://oeis.org/A374513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 14, a(n) = (2*(2*n+5)*a(n-1) + 4*(n+5)*a(n-2))
 *            / n for n >= 2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)14;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t computed_to = 1;   /* last index whose value is known */

    /* Resize cache if necessary */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache;
        if (cache == NULL) {
            new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        if (!new_cache) abort();     /* out of memory */
        cache = new_cache;
        cache_size = new_size;

        /* Initialise the base values if the cache was just created */
        if (computed_to == 1) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)14;
        }
    }

    /* Compute missing values up to n */
    for (idx_t i = computed_to + 1; i <= n; ++i) {
        seq_t term1 = cache[i - 1];
        seq_t term2 = cache[i - 2];
        seq_t numerator =
            ((seq_t)2 * (((seq_t)2 * (seq_t)i + (seq_t)5))) * term1
          + ((seq_t)4 * (((seq_t)i + (seq_t)5))) * term2;
        seq_t denom = (seq_t)i;
        cache[i] = numerator / denom;            /* exact division */
    }
    computed_to = n;

    return cache[n];
}
