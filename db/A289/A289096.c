/*
 * A289096  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 545", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A289096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-2) - 10*a(n-3) Initial values: a(0)=1,
 *            a(1)=10, a(2)=111, a(3)=1110, a(4)=11101, a(5)=111110
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0; /* one past highest computed index */

    /* Helper to grow the cache */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();

        /* Initialize base values */
        cache[0] = 1;
        cache[1] = 10;
        cache[2] = 111;
        cache[3] = 1110;
        cache[4] = 11101;
        cache[5] = 111110;
        cache_len = 6; /* highest index computed + 1 */
    } else if ((size_t)(n + 1) > cache_cap) {
        size_t newcap = cache_cap;
        while (newcap < (size_t)(n + 1))
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = newcap;
    }

    /* Compute missing values up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i >= 6) {
            /* Use recurrence: a(i) = 10*a(i-1) + a(i-2) - 10*a(i-3) */
            cache[i] = (seq_t)10 * cache[i - 1]
                     + cache[i - 2]
                     - (seq_t)10 * cache[i - 3];
        } else {
            /* Base cases already set; should not reach here */
        }
    }

    cache_len = (size_t)(n + 1);
    return cache[n];
}
