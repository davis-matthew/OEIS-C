/*
 * A361358  Expansion of x*(2 - x)/(1 - 5*x + 3*x^2 - x^3).
 * https://oeis.org/A361358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) + a(n-3) Initial values: a(1) = 2,
 *            a(2) = 9, a(3) = 39
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed terms */
    static seq_t *cache      = NULL;   /* array of terms, index 1.. */
    static idx_t  alloc_size = 0;      /* allocated array size (>= n+1) */
    static idx_t  max_comp   = 0;      /* highest index computed */

    if (n == 0) return 0;  /* offset starts at 1, but handle 0 gracefully */

    /* initialise the cache on first call */
    if (cache == NULL) {
        alloc_size = 4;                /* indices 0..3 will be used initially */
        cache = (seq_t*)malloc(sizeof(seq_t) * alloc_size);
        cache[1] = 2;
        cache[2] = 9;
        cache[3] = 39;
        max_comp = 3;
    }

    /* enlarge the cache if necessary */
    if (n + 1 > alloc_size) {
        idx_t new_size = alloc_size * 2;
        if (n + 1 > new_size) new_size = n + 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        alloc_size = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        cache[i] = 5 * cache[i-1] - 3 * cache[i-2] + cache[i-3];
    }
    if (n > max_comp) max_comp = n;

    return cache[n];
}
