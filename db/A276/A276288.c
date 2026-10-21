/*
 * A276288  a(n) = a(n-1) + 3*a(n-2) if n is even, otherwise a(n) = 3*a(n-1) +
 *          a(n-2), a(0)=0, a(1)=1.
 * https://oeis.org/A276288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1; a(n) = a(n-1) + 3*a(n-2) if n is even = 3*a(n-1)
 *            + a(n-2) if n is odd Sequence defined for n >= 0. The
 *            implementation memoizes computed values in a static array that
 *            grows on demand. It works both for fixed-width integer types and
 *            for arbitrary-precision integer classes compiled a...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, calloc, realloc, abort */
#include <stdint.h>   /* for NULL, size_t */

static seq_t *cache = NULL;       /* cache of previously computed terms */
static idx_t cache_len = 0;       /* number of elements currently cached */

/* Return a(n) for any n >= 0, memoising results. */
seq_t a(idx_t n)
{
    /* Initialise cache with the first two terms if not already done. */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t*)calloc((size_t)cache_len, sizeof(seq_t));
        if (!cache) { abort(); }
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* Extend cache to include index n if necessary. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!new_cache) { abort(); }
        cache = new_cache;

        /* Compute new terms.  cache[0] and cache[1] are already set. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if ((i & 1ULL) == 0) {                     /* even i */
                cache[i] = cache[i-1] + (seq_t)3 * cache[i-2];
            } else {                                   /* odd i */
                cache[i] = (seq_t)3 * cache[i-1] + cache[i-2];
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
