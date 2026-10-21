/*
 * A266609  Binary representation of the n-th iteration of the "Rule 41"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A266609: binary representation of the n-th iteration of Rule 41
   starting with a single ON cell.
   Formula: a(n) = 10000*a(n-2) + a(n-4) - 10000*a(n-6)   for n > 5
   Initial values:
     a(0) = 1
     a(1) = 0
     a(2) = 101
     a(3) = 1100100
     a(4) = 1
     a(5) = 11111111000
*/

#include <stdlib.h>

static seq_t *cache = NULL;    /* static cache of computed values */
static size_t cache_cap = 0;   /* size of cache (number of elements stored) */

seq_t a(idx_t n)
{
    /* Extend the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t needed = (size_t)n + 1;            /* new required size */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;                       /* allocation failed */
        cache = tmp;

        /* Compute all new terms up to n */
        for (size_t i = cache_cap; i <= n; ++i) {
            switch (i) {
                case 0: cache[0] = 1; break;
                case 1: cache[1] = 0; break;
                case 2: cache[2] = 101; break;
                case 3: cache[3] = 1100100; break;
                case 4: cache[4] = 1; break;
                case 5: cache[5] = 11111111000; break;
                default:
                    cache[i] = (seq_t)10000 * cache[i-2]
                              +     cache[i-4]
                              - (seq_t)10000 * cache[i-6];
            }
        }
        cache_cap = needed;   /* update stored size */
    }
    return cache[(size_t)n];
}
