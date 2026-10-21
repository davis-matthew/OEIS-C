/*
 * A283959  a(n) = (Sum_{j=1..h-1} a(n-j) + a(n-1)*a(n-h+1))/a(n-h) with a(1),
 *          ..., a(h)=1, where h = 5.
 * https://oeis.org/A283959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-1)*a(n-4))/a(n-5) with
 *            a(1)=...=a(5)=1, h=5
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 23, u64 n <= 41, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

#if defined(__cplusplus)
/* C++ build – arbitrary‑precision types work with normal arithmetic */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 – not used but harmless */

    /* Expand cache if necessary */
    if ((idx_t)cache_size < n) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;

        /* Compute missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 5) {
                cache[i - 1] = (seq_t)1;
            } else {
                seq_t sum  = cache[i - 2] + cache[i - 3] + cache[i - 4] + cache[i - 5];
                seq_t prod = cache[i - 2] * cache[i - 5];
                seq_t numer = sum + prod;
                seq_t denom = cache[i - 6];
                cache[i - 1] = numer / denom;   /* exact division by definition */
            }
        }
        cache_size = n;
    }
    return cache[n - 1];
}
#else
/* C build – use unsigned __int128 for intermediate results to avoid overflow */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Expand cache if necessary */
    if ((idx_t)cache_size < n) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;

        /* Compute missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 5) {
                cache[i - 1] = (seq_t)1;
            } else {
                __uint128_t a_im1 = cache[i - 2];
                __uint128_t a_im2 = cache[i - 3];
                __uint128_t a_im3 = cache[i - 4];
                __uint128_t a_im4 = cache[i - 5];
                __uint128_t prod  = a_im1 * a_im4;
                __uint128_t sum   = a_im1 + a_im2 + a_im3 + a_im4;
                __uint128_t numer = sum + prod;
                __uint128_t denom = cache[i - 6];
                cache[i - 1] = (seq_t)(numer / denom);
            }
        }
        cache_size = n;
    }
    return cache[n - 1];
}
#endif
