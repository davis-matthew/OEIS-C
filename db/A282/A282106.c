/*
 * A282106  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 427", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-3) + a(n-4) - 8*a(n-7) (n>=8) Initial values:
 *            a(0)=1, a(1)=2, a(2)=7, a(3)=2, a(4)=23, a(5)=58, a(6)=31,
 *            a(7)=162
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

/* Compute the n-th term of OEIS sequence A282106. */
seq_t a(idx_t n)
{
    /* Initial terms up to n = 7. */
    static const seq_t init[8] = { 1, 2, 7, 2, 23, 58, 31, 162 };

    if (n < 8)
        return init[n];

    /* Cache of previously computed values. */
    static seq_t *cache = NULL;
    static size_t cap    = 0;       /* number of elements allocated in cache. */
    static size_t computed = 0;     /* number of elements with valid values (0..computed-1). */

    /* Ensure that cache has room for indices 0..n. */
    ensure_cache(n, &cache, &cap);

    /* Fill the cache with the initial values if they are not yet stored. */
    if (computed < 8) {
        for (size_t i = computed; i < 8; ++i)
            cache[i] = init[i];
        computed = 8;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = computed; i <= n; ++i) {
        /* a(i) = 8*a(i-3) + a(i-4) - 8*a(i-7)  */
        cache[i] = 8 * cache[i - 3] + cache[i - 4] - 8 * cache[i - 7];
    }
    computed = n + 1;
    return cache[n];
}
