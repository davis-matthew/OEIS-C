/*
 * A389699  a(n) = Sum_{k=0..n} binomial(n+k,n-k) * Fibonacci(k+1).
 * https://oeis.org/A389699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k,n-k) * Fibonacci(k+1)
 *            Equivalent recurrence: a(0)=1,a(1)=2,a(2)=6,a(3)=20,
 *            a(n)=5*a(n-1)-7*a(n-2)+5*a(n-3)-a(n-4) for n≥4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>                    /* for abort, size_t */
#include "../utils/seq_realloc.h"      /* provides seq_realloc(seq_t*,size_t,size_t) */

static seq_t *cache      = NULL;    /* computed values */
static size_t cache_cap   = 0;      /* current capacity (number of elements) */
static idx_t  cached_up_to = 0;     /* largest n whose value is in cache */

/* Allocate or grow the cache to hold at least 'needed' elements. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    seq_t *new_cache = seq_realloc(cache, cache_cap, needed);
    if (!new_cache) {
        /* Allocation failed – terminate the program.  The build system
         * expects a terminating error when memory cannot be obtained.   */
        abort();
    }
    cache = new_cache;
    cache_cap = needed;
}

/* Initialise the first four values if they are not yet stored. */
static void init_base_values(void)
{
    if (cached_up_to < 3) {                     /* at least a(0)..a(3) required    */
        if (cache_cap < 4)
            ensure_cache(4);                    /* reserve space for four values   */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)20;
        cached_up_to = 3;
    }
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    ensure_cache(idx + 1);          /* make room for a[n] */
    init_base_values();             /* set a(0)..a(3) if missing */

    if (idx < 4)
        return cache[idx];           /* return one of the base values */

    /* Compute any missing terms up to n using the 4‑term recurrence. */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t val = (seq_t)5 * cache[(size_t)(i - 1)];
        val -= (seq_t)7 * cache[(size_t)(i - 2)];
        val += (seq_t)5 * cache[(size_t)(i - 3)];
        val -= cache[(size_t)(i - 4)];
        cache[(size_t)i] = val;
    }
    cached_up_to = n;

    return cache[idx];
}
