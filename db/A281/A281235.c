/*
 * A281235  Solutions x to the negative Pell equation y^2 = 72*x^2 - 332928
 *          with x,y >= 0.
 * https://oeis.org/A281235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n>6. Initial values: a(1)=68,
 *            a(2)=76, a(3)=132, a(4)=204, a(5)=324, a(6)=716
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 32, u64 n <= 70, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>   /* for memcpy */

seq_t a(idx_t n)
{
    if (n <= 6) {
        static const seq_t init[6] = {68, 76, 132, 204, 324, 716};
        return init[n - 1];
    }

    /* Static cache for already computed terms (0‑based index). */
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    if (cap < n) {
        /* Ensure the cache is at least n elements, but keep at least 6 for recurrence. */
        idx_t new_cap = (n > 6) ? n : 6;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failed */

        /* Copy previously computed terms. */
        if (cache) {
            memcpy(new_cache, cache, cap * sizeof(seq_t));
        }

        /* Initialise the first six values if they are not yet set. */
        if (cap < 6) {
            static const seq_t init[6] = {68, 76, 132, 204, 324, 716};
            for (idx_t i = cap; i < 6; ++i) new_cache[i] = init[i];
        }

        /* Compute new terms using the recurrence. */
        idx_t start = (cap < 6) ? 6 : cap;
        for (idx_t i = start; i < new_cap; ++i) {
            new_cache[i] = (seq_t)( (seq_t)6 * new_cache[i - 3] - new_cache[i - 6] );
        }

        free(cache);
        cache = new_cache;
        cap = new_cap;
    }

    return cache[n - 1];
}
