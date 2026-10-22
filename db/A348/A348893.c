/*
 * A348893  a(n) = 840*(2*n)!/((n + 4)!*n!).
 * https://oeis.org/A348893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 840*(2*n)!/((n + 4)!*n!)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline seq_t mul_div(seq_t x, unsigned long long num, unsigned long long den)
{
#if defined(__cplusplus)
    return (x * (seq_t)num) / (seq_t)den;
#else
    return (seq_t)(((__int128)x * (__int128)num) / (__int128)den);
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialize cache with a(0) */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)35;  /* a(0) */
    }

    /* extend cache if necessary */
    if (n >= cache_len) {
        /* increase to n+1 entries */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        while (cache_len <= n) {
            idx_t i = cache_len - 1;                     /* current index */
            unsigned long long num = 2ULL * (2ULL * i + 1ULL); /* 2*(2*i+1) */
            unsigned long long den = i + 5ULL;          /* (i+5)            */
            cache[cache_len] = mul_div(cache[i], num, den);
            cache_len++;
        }
    }

    return cache[n];
}
