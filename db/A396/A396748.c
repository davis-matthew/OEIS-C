/*
 * A396748  G.f. A(x) satisfies A(x) = 1 + x * A(x^2/(1 - x)^2) / (1 - x)^3.
 * https://oeis.org/A396748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} binomial(n+1, 2*k+2) * a(k),
 *            a(0)=1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* memoisation array */
    static size_t cache_cap = 0U;   /* allocated size of cache */

    /* Grow the cache so it contains indices 0…n */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the base case */
    if (cache[0] == 0)
        cache[0] = 1;

    /* Return the cached value if already known */
    if (cache[n] != 0)
        return cache[n];

    /* Compute all terms up to n, skipping already computed ones */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0)
            continue;

        idx_t kmax = (i - 1) / 2;
        seq_t sum = 0;

        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t bin  = binomial(i + 1, 2 * k + 2);
            sum += bin * cache[k];
        }

        cache[i] = sum;
    }

    return cache[n];
}
