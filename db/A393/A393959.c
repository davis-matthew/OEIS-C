/*
 * A393959  G.f. A(x) satisfies A(x) = 1 - E^2(log(1 - x*A(x))), where E is
 *          the Euler operator x*d/dx.
 * https://oeis.org/A393959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * a(n-1) + Sum_{k=1..n-1} (n/k) * a(k) * a(n-1-k)
 *            with a(0) = 1 (This implements the recurrence given for OEIS
 *            sequence A393959.)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of valid entries, indices 0 .. cache_size-1 */

    /* ensure cache can hold up to n */
    if ((idx_t)cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();          /* memory allocation failed */
        cache = new_cache;

        /* initialise from the first missing index up to n */
        if (cache_size == 0) {
            cache[0] = 1;                 /* a(0) */
            cache_size = 1;
        }
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            seq_t term = (seq_t)i * (seq_t)i * cache[i - 1];   /* n^2 * a(n-1) */
            seq_t sum = 0;
            for (idx_t k = 1; k < i; ++k) {
                seq_t prod = cache[k] * cache[i - 1 - k];
                prod *= (seq_t)i;          /* multiply by n first */
                prod /= (seq_t)k;          /* exact division */
                sum += prod;
            }
            cache[i] = term + sum;
        }
        cache_size = (size_t)(n + 1);
    }

    return cache[n];
}
