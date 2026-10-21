/*
 * A292185  One-fifth of the rolling arithmetic mean of the fifth powers of
 *          the natural numbers taken five at a time.
 * https://oeis.org/A292185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^5 + (n+1)^5 + (n+2)^5 + (n+3)^5 + (n+4)^5) /5) /5
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 114, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

/* Static cache holding computed values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* never used (offset 1) */

    /* Expand cache to at least n values. */
    if (cache_len < (size_t)n) {
        size_t new_len = (size_t)n;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        }
        /* Compute missing terms. */
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            if (i <= 6) {
                /* Direct evaluation using 128‑bit intermediate. */
                __uint128_t t = ( __uint128_t )i;
                __uint128_t f =
                  (((( ( (__uint128_t)5 * t + 50)* t + 300)* t + 1000)* t + 1770)* t + 1300);
                f /= 25;
                cache[i-1] = (seq_t)f;
            } else {
                /* Recurrence: a(n)=6a(n-1)-15a(n-2)+20a(n-3)-15a(n-4)+6a(n-5)-a(n-6) */
                seq_t res = mul_small(cache[i-2], 6);
                res -= mul_small(cache[i-3], 15);
                res += mul_small(cache[i-4], 20);
                res -= mul_small(cache[i-5], 15);
                res += mul_small(cache[i-6], 6);
                res -= cache[i-7];
                cache[i-1] = res;
            }
        }
        cache_len = (size_t)n;
    }
    return cache[n-1];
}
