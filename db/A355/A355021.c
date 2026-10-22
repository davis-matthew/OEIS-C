/*
 * A355021  a(n) = (-1)^n * A000032(n) - 1.
 * https://oeis.org/A355021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * A000032(n) - 1 Recurrence (for n ≥ 3): a(0) = 1,
 *            a(1) = -2, a(2) = 2, a(n) = 2*a(n-2) - a(n-3)
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 44, i64 n <= 90, i128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build system supplies seq_t and idx_t definitions. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to memoize already-computed terms so repeated calls
       in arbitrary order are fast. */
    static seq_t *cache = NULL;   /* array of computed values */
    static idx_t cached_len = 0;  /* number of valid entries: 0 .. cached_len-1 */
    static size_t cap = 0;        /* allocated capacity in elements */

    /* Ensure there is space for indices up to n */
    if (cap <= n) {
        size_t new_cap = (cap > 0) ? cap * 2 : 8;
        if (new_cap <= n + 1) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Initialize base values once */
    if (cached_len == 0) {
        cache[0] = 1;
        cache[1] = -2;
        cache[2] = 2;
        cached_len = 3;
    }

    /* Compute missing terms up to n using the recurrence */
    if (cached_len <= n) {
        for (idx_t i = cached_len; i <= n; ++i) {
            /* a(i) = 2*a(i-2) - a(i-3) */
            cache[i] = 2 * cache[i - 2] - cache[i - 3];
        }
        cached_len = n + 1;
    }

    return cache[n];
}
