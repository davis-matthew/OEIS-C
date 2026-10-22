/*
 * A364668  Domination and lower independence number of the n-Goldberg graph.
 * https://oeis.org/A364668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((11*n + 4)/5)
 * Range:     u8 n <= 115, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
#ifdef __SIZEOF_INT128__
    __int128 t = (__int128)n * 11 + 4;
    return (seq_t)(t / 5);
#else
    /* Fallback recurrence implementation for compilers without 128‑bit support */
    static seq_t *cache = NULL;
    static size_t cap  = 0;
    static size_t len  = 0;  /* number of computed terms */

    if (len == 0) {
        cap = 6;
        cache = malloc(cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 3;
        cache[2] = 5;
        cache[3] = 7;
        cache[4] = 9;
        cache[5] = 11;
        len  = 6;
    }

    if ((size_t)n >= len) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
        for (idx_t i = len; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 5] - cache[i - 6];
        }
        len = (size_t)n + 1;
    }

    return cache[n];
#endif
}
