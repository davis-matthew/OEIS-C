/*
 * A265822  If n is 0, 1, or prime, a(n) = n; else a(n) = a(n-1) + a(n-2).
 * https://oeis.org/A265822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 49, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=2; a(n)=n if n is prime, else a(n)=a(n-1)+a(n-2) */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Return small values directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of elements stored */

    /* Ensure base values are present */
    if (cache_len < 2) {
        cache_len = 2;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* If we already computed up to n, return the cached value */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache to include index n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* allocation failure */
    cache = tmp;

    /* Compute new entries sequentially */
    for (size_t i = cache_len; i < new_len; ++i) {
        if (is_prime(i))
            cache[i] = (seq_t)i;
        else
            cache[i] = cache[i - 1] + cache[i - 2];
    }

    cache_len = new_len;
    return cache[(size_t)n];
}
