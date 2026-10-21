/*
 * A290905  a(n) = (1/2)*A290904(n).
 * https://oeis.org/A290905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = 4*a(n-1) - 4*a(n-2) + 4*a(n-3) - a(n-4)
   with initial values a(0)=0, a(1)=1, a(2)=4, a(3)=12 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache  = NULL;
    static idx_t  count  = 0;   /* number of terms computed */
    static size_t capacity = 0;

    if (n >= count) {
        /* ensure capacity */
        if (capacity == 0)
            capacity = 4;
        while (capacity <= (size_t)n)
            capacity <<= 1;

        cache = (seq_t *)realloc(cache, capacity * sizeof(*cache));
        if (!cache)
            return (seq_t)0; /* allocation failure */

        /* initialise base values on first use */
        if (count == 0) {
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)4;
            cache[3] = (seq_t)12;
            count   = 4;
        }

        /* compute missing terms */
        for (idx_t i = count; i <= n; ++i) {
            size_t idx = (size_t)i;
            seq_t term = (seq_t)4 * cache[idx-1];
            term -= (seq_t)4 * cache[idx-2];
            term += (seq_t)4 * cache[idx-3];
            term -= cache[idx-4];
            cache[idx] = term;
        }

        /* update count only if we added new terms */
        if ((idx_t)(n + 1) > count)
            count = n + 1;
    }

    return cache[(size_t)n];
}
