/*
 * A292443  a(n) = (5/32)*A000045(6*n)^2.
 * https://oeis.org/A292443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 323*a(n-1) - 323*a(n-2) + a(n-3), a(0)=0, a(1)=10,
 *            a(2)=3240
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* Ensure the cache can hold index n */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first three terms on first use */
    if (cache_size == 0) {
        if (cache_cap > 0) cache[0] = (seq_t)0;
        if (cache_cap > 1) cache[1] = (seq_t)10;
        if (cache_cap > 2) cache[2] = (seq_t)3240;
        cache_size = 3;
    }

    if (n < cache_size)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];
        seq_t a_nm3 = cache[i - 3];

        if (sizeof(seq_t) <= 8) { /* integer type up to 64‑bit */
            __uint128_t diff = (__uint128_t)a_nm1 - (__uint128_t)a_nm2;
            __uint128_t temp = diff * 323u + (__uint128_t)a_nm3;
            cache[i] = (seq_t)temp;
        } else {                 /* arbitrary‑precision class */
            seq_t diff = a_nm1 - a_nm2;
            seq_t temp = diff * 323 + a_nm3;
            cache[i] = temp;
        }
    }

    cache_size = n + 1;
    return cache[n];
}
