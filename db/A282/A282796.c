/*
 * A282796  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 505", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=111, a(3)=100, a(n)=101*a(n-2)-100*a(n-4)
 *            for n>3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = (idx_t)-1;
static bool initialized = false;

seq_t a(idx_t n)
{
    /* Ensure cache can hold indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialize the first four terms once */
    if (!initialized) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        if (cache_cap > 2) cache[2] = (seq_t)111;
        if (cache_cap > 3) cache[3] = (seq_t)100;
        /* highest index that was set during initialization */
        last_computed = (cache_cap > 3 ? (idx_t)3 : (idx_t)(cache_cap - 1));
        initialized = true;
    }

    /* Compute any missing terms up to n */
    if (n > last_computed) {
        idx_t start = last_computed + 1;
        if (start < 4) start = 4;  /* recurrence starts at n=4 */
        for (idx_t i = start; i <= n; ++i) {
            seq_t term1 = ((seq_t)101) * cache[i - 2];
            seq_t term2 = ((seq_t)100) * cache[i - 4];
            cache[i] = term1 - term2;
        }
        last_computed = n;
    }

    return cache[n];
}
