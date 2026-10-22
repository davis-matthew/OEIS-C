/*
 * A309222  a(0) = 6; thereafter a(n) = a(n-1) + prime(n) if prime(n) >
 *          a(n-1), otherwise a(n) = a(n-1) - prime(n).
 * https://oeis.org/A309222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=6; for n≥1, a(n)=a(n−1)+prime(n) if prime(n)>a(n−1) else
 *            a(n−1)−prime(n)
 * Range:     u8 n <= 34, u16 n <= 3593, u32 n <= 9494, u64 n <= 9494,
 *            u128 n <= 9494, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* static cache for consecutive calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise cache with a(0) */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)6;
        cache_len = 1;
    }

    if (n == 0) {
        return cache[0];
    }

    /* grow the cache if necessary */
    if (n + 1 > cache_len) {
        size_t target = n + 1;
        if (target > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 1;
            while (new_cap < target) {
                new_cap <<= 1;
            }
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failure – return 0 as a safe fallback */
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* i is 1‑based index for prime(n) */
            uint32_t p32 = get_prime(i);          /* get nth prime, 32‑bit */
            seq_t p = (seq_t)p32;                 /* promote to seq_t */
            if (p > cache[i-1]) {
                cache[i] = cache[i-1] + p;
            } else {
                cache[i] = cache[i-1] - p;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
