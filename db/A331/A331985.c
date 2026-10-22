/*
 * A331985  a(n) is the least positive k such that n AND floor(n/k) = 0 (where
 *          AND denotes the bitwise AND operator).
 * https://oeis.org/A331985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{k ≥ 1 | n & floor(n/k) == 0}
 * Range:     u8 n <= 254, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t compute_a_value(idx_t n)
{
    idx_t k;
    for (k = 1; k <= n + 1; ++k) {
        idx_t m = n / k;
        if ((n & m) == 0)
            return (seq_t)k;
    }
    return (seq_t)(n + 1); /* never reached */
}

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of stored entries */

    if (n >= cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache)
            abort();          /* allocation failed */
        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i)
            cache[i] = compute_a_value(i);
        cache_size = n + 1;
    }
    return cache[n];
}
