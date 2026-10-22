/*
 * A302422  Number of nX3 0..1 arrays with every element equal to 0, 1, 3 or 5
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 10*a(n-3) - 8*a(n-4) for n>9 Initial values:
 *            a(1)=3, a(2)=9, a(3)=11, a(4)=21, a(5)=51, a(6)=93, a(7)=207,
 *            a(8)=517, a(9)=1007
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* lazy lazy initialization of a memoized cache */
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;   /* highest computed index in cache */

    if (n == 0)           /* safe guard: undefined behaviour for a(0) */
        return 0;

    /* One‑time setup: allocate enough space for the first 10 indices
       and populate the bootstrap terms. */
    if (!cache) {
        idx_t init_size = 10;              /* want room for indices 0..9 */
        cache = (seq_t *)malloc(sizeof(seq_t) * init_size);
        if (!cache) return 0;              /* out of memory → zero result */

        for (idx_t i = 0; i < init_size; ++i) cache[i] = 0;

        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)51;
        cache[6] = (seq_t)93;
        cache[7] = (seq_t)207;
        cache[8] = (seq_t)517;
        cache[9] = (seq_t)1007;
        cache_n = 9;
    }

    /* If the requested index is already cached, return it immediately. */
    if (n <= cache_n)
        return cache[n];

    /* Ensure the cache is large enough; double its size while needed. */
    idx_t required = n + 1;
    if (required > cache_n + 1) {
        idx_t new_size = cache_n * 2;
        if (new_size < required) new_size = required * 2;

        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (!new_cache) return 0;            /* out of memory → zero */

        for (idx_t i = 0; i <= cache_n; ++i)
            new_cache[i] = cache[i];
        for (idx_t i = cache_n + 1; i < new_size; ++i)
            new_cache[i] = 0;

        free(cache);
        cache = new_cache;
    }

    /* Extend the cache up to the requested index using the recurrence. */
    for (idx_t k = cache_n + 1; k <= n; ++k) {
        /* Recurrence holds only for k >= 10, but the loop starts at k = 10. */
        cache[k] = cache[k - 1] + 10 * cache[k - 3] - 8 * cache[k - 4];
    }
    cache_n = n;

    return cache[n];
}
