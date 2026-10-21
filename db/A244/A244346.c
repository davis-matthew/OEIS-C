/*
 * A244346  Decimal expansion of 56/13, the Korn constant for the sphere.
 * https://oeis.org/A244346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) - a(n-3) + a(n-4)
   for n > 5, with initial values
   a(1) = 4, a(2) = 3, a(3) = 0, a(4) = 7, a(5) = 6 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for already computed terms, indices 1‑based. */
    static seq_t *cache   = NULL;   /* array of a(i), i>=1            */
    static size_t cap     = 0;      /* allocated capacity (in elements) */
    static size_t maxlen  = 0;      /* number of terms currently stored */

    /* Ensure the cache can hold index n. */
    if (n > (idx_t)cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – return 0 as a safe fallback. */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initial values for indices 1 … 5. */
    if (maxlen < 5) {
        static const seq_t init[5] = { (seq_t)4,
                                       (seq_t)3,
                                       (seq_t)0,
                                       (seq_t)7,
                                       (seq_t)6 };

        size_t upto = n < 5 ? (size_t)n : 5;
        for (idx_t i = maxlen + 1; i <= upto; ++i)
            cache[i-1] = init[i-1];
        if (upto > maxlen)
            maxlen = upto;
    }

    /* Use the recurrence to fill in missing terms. */
    if (maxlen < n) {
        for (idx_t i = maxlen + 1; i <= n; ++i) {
            /* a(i) = a(i-1) - a(i-3) + a(i-4),  i > 5. */
            cache[i-1] = cache[i-2] - cache[i-4] + cache[i-5];
        }
        maxlen = n;
    }

    return cache[n-1];
}
