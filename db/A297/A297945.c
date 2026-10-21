/*
 * A297945  Number of n X 2 0..1 arrays with every element equal to 1, 2, 3 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + 2*a(n-3) - 6*a(n-4) - 4*a(n-5) (n >
 *            6) a(1) = 1, a(2) = 4, a(3) = 18, a(4) = 52, a(5) = 174, a(6) =
 *            604 The function memoises the values so it works for calls in
 *            arbitrary order
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* immutable initial values */
    static const seq_t init[7] = {0, 1, 4, 18, 52, 174, 604};

    /* static memoisation table */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated slots (index 0 unused) */
    static size_t cache_size = 0;  /* largest index stored (>= 0) */

    /* ensure capacity for index n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 32;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (cache) {
            memcpy(new_arr, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* initialise the first six terms if the table is empty */
    if (cache_size == 0) {
        for (size_t i = 1; i <= 6; ++i)
            cache[i] = init[i];
        cache_size = 6;
    }

    /* extend the table up to n */
    for (idx_t i = (idx_t)(cache_size + 1); i <= (idx_t)n; ++i) {
        if (i <= 6) continue; /* already initialised */

        /* compute using only unsigned arithmetic:
           term1 = 4*a[i-1] + 2*a[i-3];
           term2 = 2*a[i-2] + 6*a[i-4] + 4*a[i-5];
           a[i]  = term1 - term2   (term1 >= term2 for n > 6) */
        seq_t term1 = cache[i-1] * 4 + cache[i-3] * 2;
        seq_t term2 = cache[i-2] * 2 + cache[i-4] * 6 + cache[i-5] * 4;
        cache[i] = term1 - term2;
    }

    cache_size = (cache_size < (size_t)n) ? (size_t)n : cache_size;
    return cache[n];
}
