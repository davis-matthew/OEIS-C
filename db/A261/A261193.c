/*
 * A261193  a(n) = n! - 2.
 * https://oeis.org/A261193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! - 2
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* This function returns n! - 2 for any n >= 0.
   It caches factorial values up to the largest request,
   so repeated calls are fast. */
seq_t a(idx_t n)
{
    /* static cache: fact_cache[i] = i! */
    static seq_t *fact_cache = NULL;
    static idx_t fact_max = 0;          /* highest factorial computed */
    static size_t cache_size = 0;       /* allocated size of fact_cache */

    /* initialize cache for 0! if needed */
    if (fact_cache == NULL) {
        /* allocate for index 0 */
        fact_cache = (seq_t*)malloc(sizeof(seq_t));
        if (fact_cache == NULL) return (seq_t)0;  /* unlikely failure */
        fact_cache[0] = (seq_t)1;  /* 0! = 1 */
        cache_size = 1;
        fact_max = 0;
    }

    /* extend cache if we need a larger factorial */
    if (n > fact_max) {
        /* ensure storage for indices up to n */
        if (cache_size < (size_t)(n + 1)) {
            seq_t *tmp = (seq_t*)realloc(fact_cache, sizeof(seq_t) * (n + 1));
            if (tmp == NULL) return (seq_t)0;  /* unlikely failure */
            fact_cache = tmp;
            cache_size = (size_t)(n + 1);
        }
        /* compute missing factorials */
        for (idx_t i = fact_max + 1; i <= n; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_max = n;
    }

    /* a(n) = n! - 2 */
    return fact_cache[n] - (seq_t)2;
}
