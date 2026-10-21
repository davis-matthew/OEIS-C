/*
 * A267045  Decimal representation of the middle column of the "Rule 91"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+a(n-2)-2*a(n-3) for n > 4 with initial values
 *            a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 8, a(4) = 17
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: a[i] holds the value of a(i) for 0 <= i < len. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  len   = 0;
    idx_t i;

    /* Allocate and initialise first five terms on first call */
    if (cache == NULL) {
        cap = 5;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) {
            /* Allocation failed; return zero as a safe value */
            return 0;
        }
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 4;
        cache[3] = 8;
        cache[4] = 17;
        len = 5;
    }

    /* If n is already computed, return it immediately */
    if (n < len) {
        return cache[n];
    }

    /* Grow the cache if necessary */
    if ((idx_t)cap <= n) {
        size_t new_cap = (cap == 0) ? 5 : cap * 2;
        while ((idx_t)new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Reallocation failed; return zero as a safe value */
            return 0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (i = len; i <= n; ++i) {
        cache[i] = cache[i-1] * 2 + cache[i-2] - cache[i-3] * 2;
    }

    len = n + 1;
    return cache[n];
}
