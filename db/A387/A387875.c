/*
 * A387875  a(n) = Sum_{k=0..n} binomial(3*n+1,6*k).
 * https://oeis.org/A387875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 19*a(n-2) + 189*a(n-3) + 216*a(n-4) for n>3
 *            a(0)=1, a(1)=1, a(2)=8, a(3)=211
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;
    static idx_t cache_size = 0;

    if (cache_size == 0) {
        size_t init = 4; /* need indices 0..3 */
        cache = (seq_t*)calloc(init, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)211;
        cache_max = init;
        cache_size = 4;
    }

    if (n < cache_size)
        return cache[n];

    if (n >= cache_max) {
        size_t new_max = cache_max * 2;
        while (new_max <= (size_t)n) new_max *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_max * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        for (size_t i = cache_max; i < new_max; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_max = new_max;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 4) continue; /* base terms already set */
        seq_t val = (seq_t)7 * cache[i - 1];
        val -= (seq_t)19 * cache[i - 2];
        val += (seq_t)189 * cache[i - 3];
        val += (seq_t)216 * cache[i - 4];
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
