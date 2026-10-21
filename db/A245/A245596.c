/*
 * A245596  Number of tilings of an n X 1 rectangle by tiles of dimension 1 X
 *          1 and 2 X 1 such that every tile is next to a tile of different
 *          size.
 * https://oeis.org/A245596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 37, u32 n <= 71, u64 n <= 140,
 *            u128 n <= 278, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-3)+a(n-4)+a(n-5)+a(n-6)   (for n ≥ 7)
 * with initial values:
 *   a(0)=1, a(1)=0, a(2)=0, a(3)=2, a(4)=1, a(5)=1, a(6)=4
 *
 * This implementation uses a simple O(n) recurrence with a static cache
 * so that a(n) can be computed in any order of calls.  It works for
 * all supported seq_t types (unsigned integers or arbitrary‑precision).
 */

#include <stdlib.h>   /* for realloc */

/* sequence value calculation */
seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache      = NULL;  /* values a(0) … a(max_index)   */
    static idx_t  capacity   = 0;     /* number of allocated slots    */
    static int    initialized = 0;    /* flag whether base values set */

    /* initialise base cases once */
    if (!initialized) {
        /* allocate space for the first 7 terms */
        capacity = 7;
        cache = (seq_t*)realloc(NULL, capacity * sizeof(seq_t));
        for (idx_t i = 0; i < capacity; ++i) cache[i] = 0;
        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)1;
        cache[6] = (seq_t)4;
        initialized = 1;
    }

    /* expand cache if necessary */
    if (n >= capacity) {
        /* new capacity large enough to hold up to n */
        idx_t new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        /* initialise new slots to zero */
        for (idx_t i = capacity; i < new_cap; ++i) cache[i] = 0;
        /* compute missing terms using the recurrence */
        for (idx_t i = capacity; i <= n; ++i) {
            if (i <= 6) continue;  /* base terms already set */
            cache[i] = cache[i-3] + cache[i-4] + cache[i-5] + cache[i-6];
        }
        capacity = new_cap;
    }

    return cache[n];
}
