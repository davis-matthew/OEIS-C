/*
 * A351787  a(1) = 1; a(n+1) = a(n) + Sum_{d|n} a(n/d) * a(d).
 * https://oeis.org/A351787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = a(n) + Σ_{d|n} a(n/d) * a(d)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Memoisation cache for a(n). Indexes are 1‑based. */
static seq_t *cache          = NULL;
static size_t cache_cap      = 0;   /* allocated capacity (>= cache_size+1) */
static idx_t cache_size      = 0;   /* highest n with cached value */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((idx_t)(cache_cap) > n)
        return;

    size_t new_cap = cache_cap ? 2 * cache_cap : 16;
    while ((size_t)(n + 1) > new_cap)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* out of memory, terminate */

    cache = tmp;
    cache_cap = new_cap;
}

/* a(n) for n >= 1, memoised. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, return 0 as safety */

    ensure_cache(n);

    /* Compute values sequentially from the first missing index up to n. */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        if (k == 1) {
            cache[1] = (seq_t)1;
            continue;
        }

        idx_t m = k - 1;          /* sum over divisors of m */
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                idx_t e = m / d;
                seq_t prod = cache[d] * cache[e];
                sum += prod;
                if (e != d)
                    sum += prod;   /* add the symmetric divisor */
            }
        }
        cache[k] = cache[k - 1] + sum;
    }

    cache_size = n;
    return cache[n];
}
