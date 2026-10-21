/*
 * A295473  a(0) = 0; for n>0, a(n) = 9*n!.
 * https://oeis.org/A295473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n! (a(0)=0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle offset zero */
    if (n == 0) {
        return (seq_t)0;
    }

    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static seq_t current_fact = (seq_t)1;   /* factorial of fact_index */
    static idx_t fact_index = 0;            /* last factorial index computed */

    /* Allocate or grow the cache to at least n+1 entries */
    if (cache == NULL) {
        size_t cap = 1;
        while (cap <= (size_t)n) {
            cap <<= 1;
        }
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) {
            abort();
        }
        cache_cap = (idx_t)cap;
        cache[0] = (seq_t)0;                 /* a(0) is 0 */
    } else if (cache_cap <= n) {
        size_t new_cap = (size_t)cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            abort();
        }
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Compute missing factorials and sequence values */
    for (idx_t i = fact_index + 1; i <= n; ++i) {
        current_fact = current_fact * (seq_t)i;      /* i! */
        cache[i] = current_fact * (seq_t)9;          /* 9*i! */
    }
    fact_index = n;
    return cache[n];
}
