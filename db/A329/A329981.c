/*
 * A329981  a(1) = 0, and for n > 0, a(n+1) = floor(k / 3) where k is the
 *          number of terms equal to a(n) among the first n terms.
 * https://oeis.org/A329981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A329981 – ordinal transform with division by 3
   a(1) = 0
   a(n+1) = floor( o(n) / 3 )
   where o(n) is the number of occurrences of a(n) among the first n terms.
   The function caches computed terms so that calls in arbitrary order are
   answered in O( max(n_i) ) time in total. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for terms, 1‑based indexing */
    static seq_t *cache = NULL;
    static size_t  cache_len = 0;   /* highest index stored */
    static size_t  cache_cap = 0;   /* allocated capacity */

    /* static table of occurrence counts keyed by value */
    static size_t *counts = NULL;
    static size_t  counts_cap = 0;  /* allocated capacity for counts array */

    if (n == 0) return (seq_t)0;  /* undefined by sequence, return 0 */

    /* first call: initialise structures */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        counts_cap = 16;
        counts = (size_t*)calloc(counts_cap, sizeof(size_t));
        cache[1] = (seq_t)0;      /* a(1) = 0 */
        cache_len = 1;
        counts[0] = 1;            /* one occurrence of 0 so far */
    }

    /* extend cache up to requested index */
    if (cache_len < (size_t)n) {
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            seq_t prev = cache[i-1];
            seq_t k    = counts[(size_t)prev];
            seq_t next = k / (seq_t)3;   /* floor division */

            /* ensure cache capacity */
            if (cache_len + 1 >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[i] = next;

            /* ensure counts array can hold value next */
            if ((size_t)next >= counts_cap) {
                size_t new_cap = counts_cap;
                while ((size_t)next >= new_cap) new_cap *= 2;
                size_t *tmp = (size_t*)realloc(counts, new_cap * sizeof(size_t));
                for (size_t j = counts_cap; j < new_cap; ++j) tmp[j] = 0;
                counts   = tmp;
                counts_cap = new_cap;
            }
            counts[(size_t)next] += 1;
            cache_len = i;
        }
    }

    return cache[(size_t)n];
}
