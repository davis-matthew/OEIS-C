/*
 * A306357  Number of nonempty subsets of {1, ..., n} containing no three
 *          cyclically successive elements.
 * https://oeis.org/A306357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*
 * Formula: a(n) = 2*a(n-1) - a(n-4)   for n > 6
 * Initial values: a(0)=0, a(1)=1, a(2)=3, a(3)=6, a(4)=10, a(5)=20, a(6)=38
 * Memoization: values are cached in a static dynamic array so calls in any order
 * are O(1) after the first computation for a given n.
 */
seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;       /* pointer to array of seq_t */
    static size_t cache_cap = 0;      /* allocated size of the cache */
    static idx_t highest = 0;         /* highest index that has been computed */

    /* First call: allocate cache for indices 0..6 and set base values */
    if (cache == NULL) {
        cache_cap = 7;                         /* need indices 0..6 inclusive */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;           /* allocation failure, return 0 */

        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 6;
        cache[4] = 10;
        cache[5] = 20;
        cache[6] = 38;
        highest   = 6;
    }

    /* Expand cache if n is larger than any previously computed value */
    if (n > highest) {
        /* Ensure there is space for indices 0..n */
        if ((size_t)(n + 1) > cache_cap) {
            size_t new_cap = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;         /* allocation failure, fall back */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing values using the recurrence */
        for (idx_t i = highest + 1; i <= n; ++i) {
            /* a(i) = 2*a(i-1) - a(i-4) = a(i-1) + a(i-1) - a(i-4) */
            cache[i] = cache[i - 1] + cache[i - 1] - cache[i - 4];
        }
        highest = n;
    }

    return cache[n];
}
