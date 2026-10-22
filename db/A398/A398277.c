/*
 * A398277  Number of total dominating sets in the 4 X n king graph.
 * https://oeis.org/A398277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + 39*a(n-2) + 301*a(n-3) + 858*a(n-4) -
 *            388*a(n-5) - 1903*a(n-6) - 1073*a(n-7) - 4517*a(n-8) -
 *            2111*a(n-9) + 1836*a(n-10) - 3240*a(n-11) + 3672*a(n-12) -
 *            972*a(n-13) for n >= 13 Initial values: a(0)=1, a(1)=4,
 *            a(2)=165, a(3)=2545, a(4)=35458, a(5)=538977, a(6)=8213971,
 *            a(7)=12415...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial 13 terms required for the recurrence. */
    static const seq_t init[13] = {
        1ULL, 4ULL, 165ULL, 2545ULL, 35458ULL, 538977ULL,
        8213971ULL, 124153394ULL, 1877374185ULL, 28409150827ULL,
        429855925112ULL, 6503722723505ULL, 98402726217973ULL
    };

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Lazy initialisation of the cache with the first 13 values. */
    if (cache == NULL) {
        cache_cap = 13;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure; harmless */
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init[i];
        cache_len = 13;
    }

    size_t idx = (size_t)n;
    if (idx + 1 > cache_cap) {     /* grow cache if needed */
        size_t new_cap = cache_cap ? cache_cap : 13;
        while (new_cap <= idx)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return value if it is already cached. */
    if (idx < cache_len)
        return cache[idx];

    /* Compute missing values up to idx using the recurrence. */
    for (size_t k = cache_len; k <= idx; ++k) {
        if (k < 13) {  /* safety: redundant but keeps indices valid */
            cache[k] = init[k];
            continue;
        }
        /* Compute the sum of positive and negative contributions
           separately to avoid underflow on unsigned types.        */
        seq_t pos = 11 * cache[k - 1]
                  + 39 * cache[k - 2]
                  + 301 * cache[k - 3]
                  + 858 * cache[k - 4]
                  + 1836 * cache[k - 10]
                  + 3672 * cache[k - 12];

        seq_t neg = 388 * cache[k - 5]
                  + 1903 * cache[k - 6]
                  + 1073 * cache[k - 7]
                  + 4517 * cache[k - 8]
                  + 2111 * cache[k - 9]
                  + 3240 * cache[k - 11]
                  + 972 * cache[k - 13];

        cache[k] = pos - neg;      /* safe because pos >= neg by definition */
    }

    cache_len = idx + 1;
    return cache[idx];
}
