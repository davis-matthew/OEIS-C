/*
 * A351140  a(1) = 1, a(n) = smallest prime > a(n-1) + n.
 * https://oeis.org/A351140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 352, u32 n <= 244, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(n) = smallest prime > a(n-1)+n */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* 0‑based cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* current number of stored terms */
    static size_t cache_cap = 0;   /* current allocation size */

    /* initialise first term when needed */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache_len = 1;
        cache_cap = 1;
    }

    /* if the requested term is already cached, return it */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* compute terms up to n */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* enlarge buffer if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 1 : (cache_cap * 2);
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* recurrence: a(k) = smallest prime > a(k-1)+k */
        seq_t prev   = cache[k - 2];               /* a(k-1) */
        seq_t target = prev + (seq_t)k;           /* a(k-1) + k */
        seq_t cand   = target + (seq_t)1;

        while (!is_prime_big(cand)) {
            ++cand;
        }

        cache[cache_len] = cand;
        ++cache_len;
    }

    return cache[n - 1];
}
