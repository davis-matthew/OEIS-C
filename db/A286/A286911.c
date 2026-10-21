/*
 * A286911  Number of edge covers in the ladder graph P_2 x P_n.
 * https://oeis.org/A286911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 3*a(n-2) - 2*a(n-3) for n>=3 with initial
 *            values a(0)=1, a(1)=1, a(2)=7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated slots */
    static size_t len = 0;   /* number of initialized terms */

    /* ensure cache has at least n+1 slots */
    if (cap < (size_t)n + 1) {
        size_t new_cap = cap == 0 ? 4 : cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_cache == NULL)
            return (seq_t)0; /* allocation failed */
        if (cache != NULL) {
            for (size_t i = 0; i < len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cap = new_cap;
    }

    /* initialize base cases if not already done */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        len = 3;
    }

    /* compute missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i < 3) continue; /* base cases already set */
        cache[i] = (seq_t)6 * cache[i - 1] + (seq_t)3 * cache[i - 2]
                   - (seq_t)2 * cache[i - 3];
    }
    len = (size_t)n + 1;

    return cache[n];
}
