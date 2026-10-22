/*
 * A392250  a(n) = Sum_{k=0..floor(n/2)} binomial(k,2*(n-2*k)).
 * https://oeis.org/A392250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) - a(n-4) + a(n-5) Initial values: a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=0, a(4)=1
 * Range:     u8 n <= 22, u16 n <= 40, u32 n <= 76, u64 n <= 148,
 *            u128 n <= 291, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache        = NULL;
    static size_t cache_cap    = 0;
    static idx_t  last_computed = 4;

    /* Allocate cache on first call and store the five known terms. */
    if (cache == NULL) {
        size_t initial_cap = 5;
        size_t needed = (size_t)n + 1;
        if (needed > initial_cap)
            initial_cap = needed;
        cache = seq_alloc(initial_cap);
        cache_cap = initial_cap;

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)1;
    }

    /* Return immediately if the requested term is already known. */
    if (n <= last_computed)
        return cache[n];

    /* Ensure the cache is large enough to hold up to n. */
    if ((size_t)n >= cache_cap) {
        cache = seq_realloc(cache, cache_cap, (size_t)n + 1);
        cache_cap = (size_t)n + 1;
    }

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* a(i) = 2*a(i-2) - a(i-4) + a(i-5) */
        seq_t term = cache[i - 2] * (seq_t)2;
        term -= cache[i - 4];
        term += cache[i - 5];
        cache[i] = term;
    }

    last_computed = n;
    return cache[n];
}
