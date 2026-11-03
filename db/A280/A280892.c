/*
 * A280892  Squareful numbers with both neighbors squarefree.
 * https://oeis.org/A280892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9713 (2026-11-03)
 * Helpers:   is_squarefree, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280892: Squareful numbers with both neighbors squarefree.
   Computation: iterate through integers, keep those that are not squarefree
   but whose two neighbors are squarefree.
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

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
