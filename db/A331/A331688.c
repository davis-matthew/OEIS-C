/*
 * A331688  E.g.f.: exp(-x/(1 - x)) / (1 - 2*x).
 * https://oeis.org/A331688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n-3)*a(n-1) - (n-1)*(5*n-8)*a(n-2) +
 *            2*(n-1)*(n-2)^2*a(n-3) with initial values a(0)=1, a(1)=1,
 *            a(2)=3. This recurrence is used for all n >= 3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached values for memoisation. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure that cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n + 1)
        return;                          /* already available */

    idx_t new_cap = n + 1;
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – abort. */
            abort();
        }
        cache = tmp;
    }

    /* Fill in base cases or extend the cache. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)1;
        else if (i == 2)
            cache[2] = (seq_t)3;
        else {
            seq_t term1 = (seq_t)(4*i - 3) * cache[i-1];
            seq_t term2 = (seq_t)(i-1) * (seq_t)(5*i - 8) * cache[i-2];
            seq_t term3 = (seq_t)2 * (seq_t)(i-1) *
                          (seq_t)(i-2) * (seq_t)(i-2) * cache[i-3];
            cache[i] = term1 - term2 + term3;
        }
    }
    cache_size = new_cap;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
