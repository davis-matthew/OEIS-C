/*
 * A289404  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 566", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A289404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^(1+floor(n/2)) - 1)/9 Computed via the linear
 *            recurrence a(0)=1, a(1)=1, a(2)=11
 *            a(n)=a(n-1)+10*a(n-2)-10*a(n-3) for n>2 (The recurrence yields
 *            the same values as the closed form and allows correct results
 *            for any seq_t type up to the point where the value no longer
 *            fits in the type.)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialise base terms on first call */
    if (cache_len == 0) {
        cache_cap = 3;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) { /* allocation failure – abort gracefully */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)11;
        cache_len = 3;
    }

    /* Value already computed */
    if ((idx_t)cache_len > n) {
        return cache[(size_t)n];
    }

    /* Grow cache if necessary */
    size_t required = (size_t)(n + 1);
    if (required > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < required) new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort gracefully */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms via recurrence */
    for (size_t i = cache_len; i < required; ++i) {
        seq_t val = cache[i-1] + (seq_t)10 * cache[i-2] - (seq_t)10 * cache[i-3];
        cache[i] = val;
    }
    cache_len = required;

    return cache[(size_t)n];
}
