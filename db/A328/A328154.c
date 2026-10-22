/*
 * A328154  G.f. A(x) satisfies: A(x) = A(x^2) + x / (1 + x)^2.
 * https://oeis.org/A328154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(2n) = a(n) - 2n, a(2n+1) = 2n+1 (with a(1)=1)
 * Range:     i8 n <= 79, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamic cache so that repeated calls to a(n)
   are handled in O(1) after the first computation of each index.
   The cache grows lazily to the required size.  The recurrence enables
   simple O(n) filling of the cache up to the largest n requested. */

#include <stdlib.h>
#include <stdint.h>

/* External types supplied by the build system. */

static seq_t *cache = NULL;      /* cache[1..N] holds computed a(n) values */
static idx_t  cache_len = 0;     /* highest index that has been computed */

/* Expand the cache to hold values up to 'n' (inclusive) if necessary. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    /* Allocate at least n+1 elements to keep index 1-based. */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Out of memory – abort.  The caller expects no failure. */
        abort();
    }
    cache = new_cache;

    /* Compute new values in increasing order. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if ((i & 1) == 1) {
            /* odd index: a(2k+1) = 2k+1 = i */
            cache[i] = (seq_t)i;
        } else {
            /* even index: a(2k) = a(k) - 2k = a(k) - i */
            cache[i] = cache[i >> 1] - (seq_t)i;
        }
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* not used, but safe for completeness */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
