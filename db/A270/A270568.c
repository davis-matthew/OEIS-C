/*
 * A270568  Expansion of g.f. (1+4*x)/(1-8*x).
 * https://oeis.org/A270568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 12, a(n) = 8 * a(n-1) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   ensure_cache, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses an incremental dynamic cache.  For any call
 * with index n, the cache is extended to store all values up to n.
 * The recurrence is applied only for i >= 2; the base values for
 * indices 0 and 1 are set explicitly.  mul_small is used for the
 * O(1)-time multiplication by the small constant 8. */
#include <stddef.h>

/* The helper functions ensure_cache and mul_small are defined in the
 * utilities of the build environment (see the OEIS template).  They
 * are declared here via the required include path. */
#include "../utils/ensure_cache.h"
#include "../utils/mul_small.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = (idx_t)-1; /* sentinel: no value computed yet */

seq_t a(idx_t n)
{
    /* Ensure the cache has space for indices 0..n.  The helper
     * function allocates memory if necessary. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialize the base values if this is the first call or a
     * previously uninitialized cache was allocated. */
    if (max_computed == (idx_t)-1) {
        cache[0] = 1;
        max_computed = 0;
    }
    if (max_computed < 1 && n >= 1) {
        cache[1] = 12;
        max_computed = 1;
    }

    /* Compute any missing values up to n using the recurrence a(i)=8*a(i-1). */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = mul_small(cache[i - 1], 8);
    }
    max_computed = n;

    return cache[n];
}
