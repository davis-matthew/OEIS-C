/*
 * A248959  Number of ternary words of length n in which all digits 0..2 occur
 *          in every subword of 4 consecutive digits.
 * https://oeis.org/A248959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3); Initial values: a(0)=1, a(1)=3,
 *            a(2)=9, a(3)=27, a(4)=36, a(5)=72, a(6)=132
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 70, u128 n <= 143,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated capacity             */
    static size_t computed = 0;       /* highest index + 1 that has
                                         already been computed          */

    /* Make sure we have space for index n */
    ensure_cache(n, &cache, &cap);

    /* Initialise first values if not yet done */
    if (computed == 0) {
        static const seq_t base[7] = {
            (seq_t)1, (seq_t)3, (seq_t)9,
            (seq_t)27, (seq_t)36, (seq_t)72, (seq_t)132
        };
        /* Copy base values up to the available capacity */
        size_t i;
        for (i = 0; i < 7 && i < cap; ++i) {
            cache[i] = base[i];
        }
        /* Mark that we have computed up to index i-1 */
        computed = (i < 7 ? i : 7);
    }

    /* If n is already computed, return it immediately */
    if (n + 1 <= computed) {
        return cache[n];
    }

    /* Compute missing terms using the recurrence */
    size_t i;
    for (i = computed; i <= (size_t)n; ++i) {
        /* a(i) = a(i-1) + a(i-2) + a(i-3) */
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
    }
    computed = (size_t)n + 1;

    return cache[n];
}
