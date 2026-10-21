/*
 * A275778  Tribonacci-like sequence a(n) = a(n-1) + a(n-2) + a(n-3) for n >=
 *          3, with a(0) = 1, a(1) = 2, a(2) = 1.
 * https://oeis.org/A275778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) for n >= 3, a(0)=1, a(1)=2,
 *            a(2)=1
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    size_t newlen = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
    for (size_t i = cache_len; i < newlen; ++i) {
        if (i == 0)
            cache[i] = 1;
        else if (i == 1)
            cache[i] = 2;
        else if (i == 2)
            cache[i] = 1;
        else
            cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3];
    }
    cache_len = newlen;
    return cache[(size_t)n];
}
