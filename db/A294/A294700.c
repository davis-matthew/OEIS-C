/*
 * A294700  Number of permutations of [n] avoiding {1324, 2143, 3421}.
 * https://oeis.org/A294700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294700 – Number of permutations of [n] avoiding {1324,2143,3421}
   Formula: a(n) = (1/12)*(12 + 4*(3*2^n-4)*n - 5*n^2 - 2*n^3 - n^4)
   Recurrence: a(0…6) = 1,1,2,6,21,71,218
               a(n) = 9*a(n-1) - 34*a(n-2) + 70*a(n-3) - 85*a(n-4)
                      + 61*a(n-5) - 24*a(n-6) + 4*a(n-7)  (n>6)
*/

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Static cache for previously computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Initialize cache with the first seven terms */
static void init_cache(void) {
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)71;
        cache[6] = (seq_t)218;
        cache_len = 7;
    }
}

/* Grow cache capacity to at least n+1 */
static void ensure_capacity(size_t n) {
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (n >= new_cap) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Sequence generator */
seq_t a(idx_t n) {
    init_cache();

    if ((size_t)n < cache_len)
        return cache[n];

    ensure_capacity((size_t)n);

    for (size_t i = cache_len; i <= (size_t)n; ++i) {
#ifdef __cplusplus
        if constexpr (std::is_integral_v<seq_t>) {
            __int128 sum = 0;
            sum += (__int128)9 * (__int128)cache[i - 1];
            sum -= (__int128)34 * (__int128)cache[i - 2];
            sum += (__int128)70 * (__int128)cache[i - 3];
            sum -= (__int128)85 * (__int128)cache[i - 4];
            sum += (__int128)61 * (__int128)cache[i - 5];
            sum -= (__int128)24 * (__int128)cache[i - 6];
            sum += (__int128)4 * (__int128)cache[i - 7];
            cache[i] = (seq_t)sum;
        } else {
            cache[i] = (seq_t)(9 * cache[i - 1] - 34 * cache[i - 2] + 70 * cache[i - 3]
                               - 85 * cache[i - 4] + 61 * cache[i - 5]
                               - 24 * cache[i - 6] + 4 * cache[i - 7]);
        }
#else
        /* C11 mode – seq_t is an integral type */
        __int128 sum = 0;
        sum += (__int128)9 * (__int128)cache[i - 1];
        sum -= (__int128)34 * (__int128)cache[i - 2];
        sum += (__int128)70 * (__int128)cache[i - 3];
        sum -= (__int128)85 * (__int128)cache[i - 4];
        sum += (__int128)61 * (__int128)cache[i - 5];
        sum -= (__int128)24 * (__int128)cache[i - 6];
        sum += (__int128)4 * (__int128)cache[i - 7];
        cache[i] = (seq_t)sum;
#endif
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
