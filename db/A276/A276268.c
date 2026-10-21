/*
 * A276268  a(0) = a(1) = a(2) = a(3) = 1; for n>3, a(n) = (
 *          a(n-1)*a(n-2)*a(n-3) + 1 )^2 / a(n-4).
 * https://oeis.org/A276268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((a(n-1)*a(n-2)*a(n-3) + 1)^2) / a(n-4), with a(0) = a(1)
 *            = a(2) = a(3) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  computed = 0;    /* Highest index that has been computed */

    /* Initial allocation for the first four terms */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)1;
        computed = 3;
    }

    /* Resize array if the requested index exceeds current capacity */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 4) new_cap = 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        /* Initialise any new cells to zero */
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to the requested index */
    if (n > computed) {
        for (size_t i = (size_t)computed + 1; i <= (size_t)n; ++i) {
            if (i < 4) {
                cache[i] = (seq_t)1;
            } else {
                seq_t prod = cache[i-1] * cache[i-2] * cache[i-3];
                prod = prod + (seq_t)1;
                cache[i] = (prod * prod) / cache[i-4];
            }
        }
        computed = n;
    }

    return cache[(size_t)n];
}
