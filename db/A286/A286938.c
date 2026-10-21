/*
 * A286938  Length of n-th iterate of the mapping 00->001, 1->10, as in
 *          A284932.
 * https://oeis.org/A286938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 3*a(n-4) + a(n-5) a(0) =
 *            2, a(1) = 3, a(2) = 5, a(3) = 8, a(4) = 13
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage: seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Static cache for computed terms.  The cache grows as needed. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of elements currently stored */

    /* Initialise cache with the first five values. */
    if (cache_cap == 0) {
        cache_cap = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed */
        cache[0] = 2;
        cache[1] = 3;
        cache[2] = 5;
        cache[3] = 8;
        cache[4] = 13;
    }

    /* If the requested index is outside the current cache, grow it. */
    size_t idx = (size_t)n;
    if (idx >= cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;

        /* Compute missing terms using the recurrence. */
        for (size_t i = old_cap; i <= idx; ++i) {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];

            seq_t val = a1 + a1;          /* 2*a1 */
            val += a3 + a3;               /* +2*a3 */
            val += a5;                    /* +a5 */

            seq_t sub = a2;
            sub += a4 + a4 + a4;          /* +3*a4 */
            val -= sub;                   /* 2*a1 - a2 + 2*a3 - 3*a4 + a5 */

            cache[i] = val;
        }
    }

    return cache[idx];
}
