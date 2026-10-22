/*
 * A318609  a(1) = 2, a(2) = 4, a(3) = 6; for n > 3, a(n) = 3*a(n-1) -
 *          3*a(n-2) + 9*a(n-3).
 * https://oeis.org/A318609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) – 3*a(n-2) + 9*a(n-3)   for n > 3
   Initial values: a(1) = 2, a(2) = 4, a(3) = 6
   Offset: 1 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Persistent cache of computed values */
    static seq_t *cache     = NULL;
    static idx_t capacity   = 0;          /* allocated size (number of entries) */
    static idx_t up_to      = 0;          /* maximum n that has been computed */

    /* ---------- initial allocation ---------- */
    if (cache == NULL) {
        /* Preallocate space for indices 0..3 */
        capacity = 4;
        cache = (seq_t *)malloc((size_t)capacity * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;                /* allocation failure */

        cache[0] = (seq_t)0;               /* unused entry for convenience */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)6;
        up_to   = 3;
    }

    /* ---------- grow cache if needed ---------- */
    if ((idx_t)(n + 1) > capacity) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;               /* allocation failure */

        cache = tmp;
        capacity = new_cap;
    }

    /* ---------- return cached value if present ---------- */
    if (n <= up_to)
        return cache[n];

    /* ---------- compute and cache missing terms ---------- */
    for (idx_t i = up_to + 1; i <= n; ++i) {
        seq_t v = cache[i - 1] * (seq_t)3;
        v -= cache[i - 2] * (seq_t)3;
        v += cache[i - 3] * (seq_t)9;
        cache[i] = v;
    }
    up_to = n;

    return cache[n];
}
