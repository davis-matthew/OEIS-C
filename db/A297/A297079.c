/*
 * A297079  Number of n X 2 0..1 arrays with no 1 adjacent to 2 king-move
 *          neighboring 1s.
 * https://oeis.org/A297079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297079
   Recurrence:  a(n) = 2*a(n-1) + 4*a(n-2) + 4*a(n-3)
                      - 14*a(n-4) - 14*a(n-5) - 4*a(n-6)
   Initial values (offset 1):
     a(1)=4, a(2)=12, a(3)=30, a(4)=96, a(5)=286, a(6)=848
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size */
    static idx_t max_computed = 0;  /* largest n for which cache[n] is valid */

    if (n == 0) return 0; /* defensively handle offset 0 */

    /* Ensure cache can hold up to n */
    if ((size_t)n + 2 > cap) {
        size_t new_cap = cap ? cap * 2 : 64;
        if (new_cap < (size_t)n + 2) new_cap = (size_t)n + 2;
        seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!ptr) abort();      /* out of memory */
        cache = ptr;
        cap   = new_cap;
    }

    /* Initialize base values once */
    if (max_computed == 0) {
        cache[0] = 0;            /* unused */
        cache[1] = 4;
        cache[2] = 12;
        cache[3] = 30;
        cache[4] = 96;
        cache[5] = 286;
        cache[6] = 848;
        max_computed = 6;
    }

    /* Return known value if already computed */
    if (n <= max_computed) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term = 2 * cache[i-1] + 4 * cache[i-2] + 4 * cache[i-3];
        term -= 14 * cache[i-4];
        term -= 14 * cache[i-5];
        term -= 4 * cache[i-6];
        cache[i] = term;
    }
    max_computed = n;
    return cache[n];
}
