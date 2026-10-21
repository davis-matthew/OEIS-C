/*
 * A272569  A variation on Stern's diatomic sequence.
 * https://oeis.org/A272569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 0; * For n ≥ 3: * if n is even: a(n) = a(n/2);
 *            * if n is odd : a(n) = a(n/2) + a(n/2+1) +
 *            sqrt(4*a(n/2)*a(n/2+1)+1) * * Implementation uses memoised
 *            recursion; caching is shared across calls. * floor_sqrt() (from
 *            ../utils/floor_sqrt.h) provides the required square‑root * for
 *            arbi...
 * Range:     u8 n <= 36, u16 n <= 76, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/floor_sqrt.h"

static seq_t *cache  = NULL;      /* cached values, index 0 unused          */
static unsigned char *flags = NULL;/* 1 if cache[n] is valid, 0 otherwise      */
static size_t cache_cap = 0;        /* largest index currently allocated + 1 */

/* enlarge the cache to hold at least indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < (size_t)n + 1)
        new_cap <<= 1;               /* grow geometrically */

    seq_t *new_cache = (seq_t *)realloc(cache,
                                        (new_cap + 1) * sizeof(seq_t));
    unsigned char *new_flags = (unsigned char *)realloc(flags,
                                                        (new_cap + 1) * sizeof(unsigned char));
    if (!new_cache || !new_flags)
        exit(1);                     /* unrecoverable */

    /* initialise new entries to zero/false */
    if (new_cap >= cache_cap) {
        memset(new_cache + cache_cap + 1,
               0,
               (new_cap - cache_cap) * sizeof(seq_t));
        memset(new_flags + cache_cap + 1,
               0,
               (new_cap - cache_cap) * sizeof(unsigned char));
    }

    cache      = new_cache;
    flags      = new_flags;
    cache_cap  = new_cap;

    /* ensure base values are set */
    if (cache_cap >= 2) {
        cache[1] = 0; flags[1] = 1;
        cache[2] = 0; flags[2] = 1;
    }
}

/* Recursive helper that uses the cache */
static seq_t rec(idx_t n)
{
    if (n <= 2)
        return 0;                    /* base cases handled by caller */

    if (flags[n])
        return cache[n];

    seq_t result;
    if ((n & 1u) == 0) {            /* n even */
        result = rec(n >> 1);
    } else {                        /* n odd */
        idx_t k = n >> 1;           /* floor(n/2) */
        seq_t left  = rec(k);
        seq_t right = rec(k + 1);

        /* compute sqrt(4*left*right+1) */
        seq_t prod = left;
        prod *= right;
        prod *= 4;
        prod += 1;
        seq_t s = floor_sqrt(prod);

        result = left + right + s;
    }

    cache[n] = result;
    flags[n] = 1;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined, but guard anyway */

    ensure_capacity(n);
    return rec(n);
}
