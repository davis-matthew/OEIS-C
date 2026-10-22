/*
 * A334706  Number of collinear triples in a 4 X n rectangular grid.
 * https://oeis.org/A334706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334706: Number of collinear triples in a 4×n rectangular grid
   Recurrence (for n > 7):
       a(n) = 2*a(n-1) - a(n-3) - a(n-4) + 2*a(n-6) - a(n-7)
   Initial values:
       a(1)=4, a(2)=8, a(3)=20, a(4)=44, a(5)=84,
       a(6)=140, a(7)=224
   The function returns a(n) for any n ≥ 1, memoising results in a static
   cache so that calls in any order execute in O(total n) time.  It works
   for arbitrary-precision seq_t as well as for 64‑bit/earlier integer
   types.  No external helper functions are required for this implementation.
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache holds values for indices 0 .. max_computed.
       cache[0] is unused and set to 0 for convenience.          */
    static seq_t *cache = NULL;
    static size_t  cap     = 0;    /* allocated size (number of elements)   */
    static size_t  max_computed = 0; /* highest index with a valid value      */

    /* Ensure the cache has room for at least max(n,7)+1 elements,
       because a(7) is the highest initial non‑zero index.      */
    size_t needed = (size_t)n + 1;
    if (needed < 8) needed = 8;          /* at least indices 0..7                */
    if (cache == NULL) {
        /* First allocation: grow to 'needed' size.            */
        cap = needed;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) return 0; /* allocation failure: undefined behaviour tolerated */

        /* initialise index 0 (unused) and the first seven terms   */
        cache[0] = 0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)20;
        cache[4] = (seq_t)44;
        cache[5] = (seq_t)84;
        cache[6] = (seq_t)140;
        cache[7] = (seq_t)224;
        max_computed = 7;
    } else if (needed > cap) {
        /* Grow the cache exponentially until it can hold 'needed'.  */
        size_t new_cap = cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n (if necessary).                */
    if ((size_t)n > max_computed) {
        for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
            if (i <= 7) {
                /* already initialised */
                continue;
            }
            /* recurrence: a(i) = 2*a(i-1) - a(i-3) - a(i-4) + 2*a(i-6) - a(i-7) */
            seq_t term = 2 * cache[i - 1] - cache[i - 3] - cache[i - 4]
                       + 2 * cache[i - 6] - cache[i - 7];
            cache[i] = term;
        }
        max_computed = (size_t)n;
    }

    /* n is guaranteed to be ≥ 1.   */
    return cache[(size_t)n];
}
