/*
 * A281698  a(n) = 5*2^(n-1) + 2^(2*n-1) + 6^n + 1.
 * https://oeis.org/A281698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^(n-1) + 2^(2*n-1) + 6^n + 1 (offset 0) Recurrence:
 *            a(n) = 13*a(n-1) - 56*a(n-2) + 92*a(n-3) - 48*a(n-4), n>3
 *            Initial values: a(0)=5, a(1)=14, a(2)=55, a(3)=269
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_max = 0;

    /* Initialize cache with the first four terms */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)55;
        cache[3] = (seq_t)269;
        computed_max = 3;
    }

    /* If the requested index is already computed, return it */
    if (n <= (idx_t)computed_max) {
        return cache[n];
    }

    /* Resize the cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n)
            new_size <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute new terms using the recurrence */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        seq_t val = (seq_t)13 * cache[k-1];
        val -= (seq_t)56 * cache[k-2];
        val += (seq_t)92 * cache[k-3];
        val -= (seq_t)48 * cache[k-4];
        cache[k] = val;
    }

    computed_max = n;
    return cache[n];
}
