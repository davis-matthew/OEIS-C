/*
 * A266842  Decimal representation of the n-th iteration of the "Rule 69"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-2) - 84*a(n-4) + 64*a(n-6) for n>5 initial values:
 *            a(0)=1, a(1)=2, a(2)=20, a(3)=43, a(4)=336, a(5)=687
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0; /* number of elements currently stored (valid indices 0..cache_len-1) */

    /* ensure cache is initialized */
    if (cache == NULL) {
        size_t init_len = 6; /* we store up to index 5 */
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)20;
        cache[3] = (seq_t)43;
        cache[4] = (seq_t)336;
        cache[5] = (seq_t)687;
        cache_len = init_len;
    }

    /* grow cache if needed */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 6) continue;            /* initial values already set */
            /* a(i) = 21*a(i-2) + 64*a(i-6) - 84*a(i-4) */
            seq_t term = 21 * cache[i - 2] + 64 * cache[i - 6];
            term -= 84 * cache[i - 4];
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[n];
}
