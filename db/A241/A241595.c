/*
 * A241595  a(n) = -(8*a(n-4)*a(n-1) - 57*a(n-3)*a(n-2))/a(n-5) with initial
 *          values -1, 0, 1, -1, -8, 57, 455.
 * https://oeis.org/A241595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(8*a(n-4)*a(n-1) - 57*a(n-3)*a(n-2))/a(n-5) (n >= 7)
 *            with initial values: a(0) = -1, a(1) = 0, a(2) = 1, a(3) = -1,
 *            a(4) = -8, a(5) = 57, a(6) = 455. a(7) = -(8*a(3)*a(6) -
 *            57*a(4)*a(5))/a(2) and so on
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 9, i64 n <= 13, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* small terms */
    static const seq_t init[7] = { -1, 0, 1, -1, -8, 57, 455 };
    if (n < 7) return init[n];

    /* memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last_valid = 6;

    if (cache == NULL) {
        size_t newcap = (size_t)n + 1;
        if (newcap < 16) newcap = 16;
        cache = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!cache) abort();
        cap = newcap;
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
    } else if (n + 1 > cap) {
        size_t newcap = cap * 2;
        if (newcap <= (size_t)n) newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    for (idx_t i = last_valid + 1; i <= n; ++i) {
#ifdef __cplusplus
        /* Big-integer build: use normal arithmetic */
        seq_t term1 = 8 * cache[i-4] * cache[i-1];
        seq_t term2 = 57 * cache[i-3] * cache[i-2];
        cache[i] = -(term1 - term2) / cache[i-5];
#else
        /* Built-in integer build: use 128‑bit intermediates to avoid overflow */
        __int128 t1 = (__int128)cache[i-4] * cache[i-1] * 8;
        __int128 t2 = (__int128)cache[i-3] * cache[i-2] * 57;
        __int128 num = -(t1 - t2);
        cache[i] = (seq_t)(num / (__int128)cache[i-5]);
#endif
    }
    last_valid = n;
    return cache[n];
}
