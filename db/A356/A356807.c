/*
 * A356807  Tetranacci sequence beginning with 3, 7, 12, 24.
 * https://oeis.org/A356807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4); Initial values:
 *            a(1)=3, a(2)=7, a(3)=12, a(4)=24
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 66, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  max_computed = 0;

    /* Return directly for the first four terms. */
    if (n <= 4) {
        switch ((unsigned)n) {
            case 1: return (seq_t)3;
            case 2: return (seq_t)7;
            case 3: return (seq_t)12;
            case 4: return (seq_t)24;
            default: return (seq_t)0; /* unreachable – for safety */
        }
    }

    /* Allocate / grow cache as needed. */
    if (cache == NULL || cap <= (size_t)n) {
        size_t new_cap = (cap == 0 ? 16 : cap * 2);
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed – return 0 fallback */
        cache = tmp;
        cap   = new_cap;
    }

    /* Ensure the base values are initialized. */
    if (max_computed < 4) {
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)24;
        max_computed = 4;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];
    }
    max_computed = n;

    return cache[n];
}
