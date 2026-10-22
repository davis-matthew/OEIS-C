/*
 * A373707  Expansion of e.g.f. exp(x * (1 + x^3)^2).
 * https://oeis.org/A373707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 8*(n-1)*(n-2)*(n-3)*a(n-4) * +
 *            7*(n-1)*(n-2)*(n-3)*(n-4)*(n-5)*(n-6)*a(n-7) * Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=49, a(5)=241, a(6)=721,
 *            a(7)=6721
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
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

    /* Initial population of the cache */
    if (cache_len == 0) {
        cache_len = 7;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1; cache[1] = 1; cache[2] = 1; cache[3] = 1;
        cache[4] = 49; cache[5] = 241; cache[6] = 721;
    }

    /* Return already computed value */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache up to n */
    cache = (seq_t*)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 7) continue;              /* should not occur */
        seq_t sn = (seq_t)i;

        seq_t term1 = cache[i - 1];

        seq_t prod2 = 8;
        prod2 *= (sn - 1);
        prod2 *= (sn - 2);
        prod2 *= (sn - 3);
        prod2 *= cache[i - 4];

        seq_t prod3 = 7;
        prod3 *= (sn - 1);
        prod3 *= (sn - 2);
        prod3 *= (sn - 3);
        prod3 *= (sn - 4);
        prod3 *= (sn - 5);
        prod3 *= (sn - 6);
        prod3 *= cache[i - 7];

        cache[i] = term1 + prod2 + prod3;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
