/*
 * A270082  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 62", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-4)-a(n-6) for n > 11; initial values (n =
 *            0..11): 4, 7, 8, 12, 12, 24, 4, 28, 12, 44, 4, 44
 * Range:     u8 n <= 64, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static memory to cache previously computed terms */
    static seq_t *cache = NULL;
    static size_t size = 0;   /* number of stored terms (valid indices are 0..size-1) */
    static size_t cap  = 0;   /* allocated capacity */

    /* initial known values for indices 0 .. 11 */
    static const seq_t init[12] = {
        4, 7, 8, 12, 12, 24, 4, 28, 12, 44, 4, 44
    };
    if (cap == 0) {
        /* allocate a little more than needed to allow growth */
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;  /* out of memory – caller should handle */

        /* copy the initial values */
        for (size_t i = 0; i < 12; ++i) cache[i] = init[i];
        size = 12;
    }

    /* extend cache if necessary */
    while (size <= n) {
        /* allocate more space if needed */
        if (size >= cap) {
            cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
            if (!tmp) { free(cache); cache = NULL; return 0; }
            cache = tmp;
        }
        /* compute next term using the recurrence */
        /* for indices >= 12, the recurrence: a(i) = a(i-2)+a(i-4)-a(i-6) */
        cache[size] = cache[size-2] + cache[size-4] - cache[size-6];
        ++size;
    }

    return cache[n];
}
