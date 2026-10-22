/*
 * A374882  Expansion of e.g.f. exp( (1 - (1 - 9*x)^(1/3))/3 ).
 * https://oeis.org/A374882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*(n-2)*a(n-1) - 9*(3*n-8)*(3*n-7)*a(n-2) + a(n-3); a(0)
 *            = 1, a(1) = 1, a(2) = 7
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms          */
    static idx_t cache_size = 0;  /* number of allocated slots        */
    static idx_t max_computed = 0;/* largest index that has been computed */

    /* First call: initialise the first three terms */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t*)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) exit(1);          /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        max_computed = 2;
    }

    /* If the requested value is already cached, return it */
    if (n <= max_computed)
        return cache[n];

    /* Grow the cache array so it can hold indices up to n */
    if ((size_t)(cache_size) <= (size_t)n) {
        size_t new_size = (size_t)cache_size;
        while ((size_t)(n + 1) > new_size)
            new_size <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);          /* out of memory */
        cache = tmp;
        cache_size = (idx_t)new_size;
    }

    /* Compute terms from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* The recurrence is valid only for i >= 3 */
        seq_t term1 = (seq_t)18 * ((seq_t)(i - 2)) * cache[i - 1];
        seq_t term2 = (seq_t)9 * ((seq_t)(3 * i - 8))
                      * ((seq_t)(3 * i - 7)) * cache[i - 2];
        cache[i] = term1 - term2 + cache[i - 3];
    }

    max_computed = n;
    return cache[n];
}
