/*
 * A400512  Expansion of g.f. x*(1-x)/((1-4*x)*(1-5*x-5*x^2)).
 * https://oeis.org/A400512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = 9*a(n-1) – 15*a(n-2) – 20*a(n-3) with a(0)=0, a(1)=1,
 *            a(2)=8
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 0;    /* highest computed index */

    /* Ensure we have space for n and at least 3 entries */
    if (cache_cap <= (size_t)n || cache_cap < 3) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n || new_cap < 3)
            new_cap <<= 1;          /* double until large enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first three known terms once */
    if (cache_last < 2) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        cache_last = 2;
    }

    if (n <= cache_last)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t t1 = (seq_t)9 * cache[(size_t)(i-1)];
        seq_t t2 = (seq_t)15 * cache[(size_t)(i-2)];
        seq_t t3 = (seq_t)20 * cache[(size_t)(i-3)];
        cache[(size_t)i] = t1 - t2 - t3;
    }
    cache_last = n;
    return cache[(size_t)n];
}
