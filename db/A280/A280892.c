/*
 * A280892  Squareful numbers with both neighbors squarefree.
 * https://oeis.org/A280892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280892: Squareful numbers with both neighbors squarefree.
   Computation: iterate through integers, keep those that are not squarefree
   but whose two neighbors are squarefree.
*/

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/append_candidate.h"

/* External linkage function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* Undefined, but safe to return 0 */

    /* Cached terms */
    static seq_t *cache   = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;

    /* Last number that was examined during a previous call.  Start at 3
       so that the first candidate is 4. */
    static seq_t last_checked = 3;

    /* Extend the cache until we have at least n terms. */
    while (cache_len < n) {
        /* Next integer to test. */
        seq_t x = ++last_checked;

        /* Check the required properties. */
        if (!is_squarefree(x)            /* x is squareful */
            && is_squarefree(x - 1)      /* x-1 is squarefree */
            && is_squarefree(x + 1)) {   /* x+1 is squarefree */
            /* Append to cache. */
            append_candidate(&cache, &cache_len, &cache_cap, x);
        }
    }

    return cache[n - 1];
}
