/*
 * A324442  a(n) = Product_{i=1..n, j=1..n} (i^2 + j).
 * https://oeis.org/A324442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=1..n} Π_{j=1..n} (i^2 + j)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already-computed terms */
    static seq_t *cache = NULL;   /* array of seq_t values */
    static idx_t cache_len = 0;   /* number of entries stored (cache_len > 0 when cache==NULL) */

    /* Ensure cache is initialized with a(0) = 1 */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
    }

    /* If requested n is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Expand cache up to n, computing new terms on the fly */
    size_t old_len = (size_t)cache_len;
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) abort();          /* out of memory */
    cache = new_cache;
    cache_len = (idx_t)new_len;

    /* Compute a(k) for k = old_len .. n */
    for (idx_t k = old_len; k <= n; ++k) {
        seq_t prod = (seq_t)1;
        seq_t k_sq = (seq_t)k * (seq_t)k;

        /* product over i: (i^2 + k), for i = 1 .. k-1 */
        for (idx_t i = 1; i < k; ++i) {
            seq_t ii = (seq_t)i * (seq_t)i;
            prod *= ii + (seq_t)k;
        }

        /* product over j: (k^2 + j), for j = 1 .. k-1 */
        for (idx_t j = 1; j < k; ++j) {
            prod *= k_sq + (seq_t)j;
        }

        /* multiply by the term for (i = k, j = k) */
        prod *= k_sq + (seq_t)k;

        /* accumulate into a(k) */
        cache[k] = cache[k - 1] * prod;
    }

    return cache[n];
}
