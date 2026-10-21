/*
 * A272743  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 526", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) for n>4 Initial values: a(0)=1,
 *            a(1)=5, a(2)=17, a(3)=69, a(4)=277 The function computes the
 *            term by recurrence and memoizes already computed values for
 *            efficient repeated calls
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t cache_last = (idx_t)-1;  /* highest index computed so far */

    /* lazy initialization */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();  /* allocation failure */
        cache[0] = 1;
        cache[1] = 5;
        cache[2] = 17;
        cache[3] = 69;
        cache[4] = 277;
        cache_last = 4;
    }

    if (n <= cache_last) {
        return cache[(size_t)n];
    }

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = cache_size * 2;
        while ((size_t)(n + 1) > new_size) {
            new_size *= 2;
        }
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) abort();  /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing values using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[(size_t)i] = 5 * cache[(size_t)(i - 1)] -
                           4 * cache[(size_t)(i - 2)];
    }
    cache_last = n;
    return cache[(size_t)n];
}
