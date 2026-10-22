/*
 * A300445  a(n) is the maximum value of the quartet index of a bifurcating
 *          rooted tree with n leaves.
 * https://oeis.org/A300445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(floor(n/2)) + a(ceil(n/2)) +
 *            binomial(floor(n/2),2)*binomial(ceil(n/2),2) with
 *            a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 12, u16 n <= 44, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

#ifdef __cplusplus
# include <new>
#endif

/* Recursive helper with memoisation */
static seq_t _a_recursive(idx_t n)
{
    /* Static arrays for memoisation */
    static seq_t *cache = NULL;
    static bool *computed = NULL;
    static size_t cache_cap = 0; /* number of slots allocated for cache & computed */

    /* Ensure we have space for indices up to n (including 0..n) */
    if (cache_cap <= n) {
        size_t new_cap = n + 1;
        /* Resize cache */
        seq_t *tmp_cache;
        bool *tmp_comp;

        /* Realloc cache array (cast for C++) */
        tmp_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp_cache;
        /* If the type has a constructor, initialise new elements */
#ifdef __cplusplus
        for (size_t i = cache_cap; i < new_cap; ++i)
            new (&cache[i]) seq_t();
#endif

        /* Realloc computed flag array */
        tmp_comp = (bool*)realloc(computed, new_cap * sizeof(bool));
        computed = tmp_comp;
        /* Initialise new flag entries */
        memset(computed + cache_cap, 0, (new_cap - cache_cap) * sizeof(bool));

        cache_cap = new_cap;
    }

    /* Base cases */
    if (n <= 3) {
        if (!computed[n]) {
            cache[n] = 0;
            computed[n] = true;
        }
        return cache[n];
    }

    /* Return cached value if already computed */
    if (computed[n])
        return cache[n];

    /* Recurrence */
    idx_t left  = n >> 1;          /* floor(n/2) */
    idx_t right = n - left;        /* ceil(n/2)  */

    seq_t a_left  = _a_recursive(left);
    seq_t a_right = _a_recursive(right);

    seq_t bin_left  = binomial(left, 2);
    seq_t bin_right = binomial(right, 2);

    seq_t result = a_left + a_right + (bin_left * bin_right);

    cache[n] = result;
    computed[n] = true;
    return result;
}

/* Public API required by the build system */
seq_t a(idx_t n)
{
    return _a_recursive(n);
}
