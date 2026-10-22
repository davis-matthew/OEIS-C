/*
 * A367637  G.f. A(x) satisfies A(x) = 1 / (1 - x * A(x^6)).
 * https://oeis.org/A367637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/6)} a(k) * a(n-1-6*k), a(0) = 1
 * Range:     u8 n <= 26, u16 n <= 48, u32 n <= 92, u64 n <= 180,
 *            u128 n <= 357, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"
#include "../utils/seq_alloc.h"

static seq_t *cache = NULL;      /* cache array of computed values      */
static idx_t cached_cap = 0;     /* current capacity of 'cache'        */
static idx_t cached_len = 0;     /* number of values already filled    */

/* Ensure the cache can hold values up to index 'n' (inclusive).  */
static void
ensure_cache(idx_t n)
{
    if (cached_cap <= n) {
        /* Grow capacity, doubling until it exceeds 'n'. */
        idx_t new_cap = (cached_cap == 0) ? 1 : cached_cap * 2;
        while (new_cap <= n) new_cap *= 2;
        /* Reallocate using the helper that knows how to handle
           arbitrary-precision seq_t types. */
        cache = seq_realloc(cache, (size_t)cached_cap, (size_t)new_cap);
        cached_cap = new_cap;
    }
}

seq_t
a(idx_t n)
{
    /* If we already have computed up to 'n', just return it. */
    if (n < cached_len)
        return cache[n];

    /* Make sure the cache can hold all indices up to 'n'. */
    ensure_cache(n);

    /* Compute all missing values sequentially. */
    for (idx_t i = cached_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;          /* a(0) = 1 */
        } else {
            seq_t sum = 0;
            idx_t maxk = (i - 1) / 6;     /* floor((i-1)/6) */
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t j = i - 1 - 6 * k;
                sum += cache[k] * cache[j];
            }
            cache[i] = sum;
        }
    }
    cached_len = n + 1;
    return cache[n];
}
