/*
 * A281503  Solutions x to the negative Pell equation y^2 = 33*x^2 - 8 with
 *          x,y >= 0.
 * https://oeis.org/A281503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A281503: positive integers x such that y² = 33·x² – 8.
 *
 * Recurrence (for n > 4):
 *   a(n) = 46·a(n-2) – a(n-4)
 *
 * Initial terms (offset 1):
 *   a(1) = 1,  a(2) = 3,  a(3) = 43,  a(4) = 137
 *
 * The implementation stores computed values in a static cache so that
 * calls with arbitrary n order are answered in O(n) time with minimal
 * reallocation cost.
 */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; not used by tests. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache of computed values. */
    static seq_t *cache      = NULL;
    static idx_t  cap       = 0;   /* allocated size (≥ last computed n + 1) */
    static idx_t  last_done = 0;   /* largest n that has been computed */

    /* Expand cache if necessary. */
    if (cap < n + 1) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)               /* allocation failure – return 0 */
            return (seq_t)0;
        cache = tmp;
        cap   = (idx_t)new_cap;
    }

    /* Compute missing values up to n. */
    for (idx_t i = last_done + 1; i <= n; ++i) {
        if (i == 1)
            cache[1] = (seq_t)1;
        else if (i == 2)
            cache[2] = (seq_t)3;
        else if (i == 3)
            cache[3] = (seq_t)43;
        else if (i == 4)
            cache[4] = (seq_t)137;
        else
            cache[i] = (seq_t)(46 * cache[i - 2] - cache[i - 4]);
    }
    last_done = n;

    return cache[n];
}
