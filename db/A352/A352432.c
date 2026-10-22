/*
 * A352432  Number of tilings of a 4 X n rectangle using dominoes and 2 X 2
 *          tiles.
 * https://oeis.org/A352432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 10*a(n-2) - 18*a(n-3) - 14*a(n-4) + 20*a(n-5);
 *            a(0) = 1, a(1) = 1, a(2) = 11, a(3) = 29, a(4) = 165
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t cache_size   = 0;
    static idx_t last_computed = 4; /* indices 0..4 are initialized */

    /* allocate initial cache if needed */
    if (!cache) {
        size_t init_size = (size_t)n + 1;
        if (init_size < 5) init_size = 5;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        cache_size = init_size;

        /* set initial values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)29;
        cache[4] = (seq_t)165;
        last_computed = 4;
    }
    /* resize cache if larger n requested */
    else if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    if (n <= last_computed) return cache[n];
    if (n <= 4)                 return cache[n];

    /* compute entries from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term = cache[i - 1] * (seq_t)3;
        term += cache[i - 2] * (seq_t)10;
        term -= cache[i - 3] * (seq_t)18;
        term -= cache[i - 4] * (seq_t)14;
        term += cache[i - 5] * (seq_t)20;
        cache[i] = term;
    }
    last_computed = n;
    return cache[n];
}
