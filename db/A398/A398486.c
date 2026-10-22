/*
 * A398486  Numbers k such that cos(k) > cos(k+1) > cos(k+2) > cos(k+3) >
 *          cos(k+4).
 * https://oeis.org/A398486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer k such that cos(k) > cos(k+1) >
 *            cos(k+2) > cos(k+3) > cos(k+4). This holds iff floor((k+0.5)/π)
 *            == floor((k+3.5)/π) and that value is even
 * Range:     u8 n <= 6, u16 n <= 1477, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Static cache so calls can arrive in any order. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_k = 0;

    /* Ensure space for the requested terms. */
    if (cache_len < (size_t)n) {
        /* Allocate or grow the cache. */
        if (cache == NULL) {
            cache_cap = 128;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_cap < (size_t)n) {
            while (cache_cap < (size_t)n) cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* π as a rational number: π ≈ π_num / π_den
         *  π_num = π * 10^15  (≈ 3.141592653589793 × 10^15)
         *  π_den = 10^15
         */
        const unsigned long long PI_NUM  = 3141592653589793ULL;   /* π * 10^15          */
        const unsigned long long PI_DEN  = 1000000000000000ULL;   /* 10^15               */
        const unsigned long long TWO_PI = PI_NUM * 2ULL;         /* 2π * 10^15          */
        const unsigned long long DEN    = TWO_PI;                /* denominator for division */

        /* Generate missing terms. */
        while (cache_len < (size_t)n) {
            /* Find the next k satisfying the condition. */
            while (1) {
                /* floor((k+0.5)/π) */
                unsigned __int128 a1   = (unsigned __int128)next_k * 2ULL + 1ULL;
                unsigned __int128 val1 = a1 * (unsigned __int128)PI_DEN;
                unsigned long long r1  = (unsigned long long)(val1 / (unsigned __int128)DEN);

                /* floor((k+3.5)/π) */
                unsigned __int128 a2   = (unsigned __int128)next_k * 2ULL + 7ULL;
                unsigned __int128 val2 = a2 * (unsigned __int128)PI_DEN;
                unsigned long long r2  = (unsigned long long)(val2 / (unsigned __int128)DEN);

                if (r1 == r2 && ((r1 & 1ULL) == 0ULL)) {
                    cache[cache_len++] = next_k;
                    next_k++;                      /* prepare for next search */
                    break;
                }
                next_k++;
            }
        }
    }

    return cache[n - 1];
}
