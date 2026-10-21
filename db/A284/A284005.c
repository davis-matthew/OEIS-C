/*
 * A284005  a(0) = 1, and for n > 1, a(n) = (1 + A000120(n))*a(floor(n/2));
 *          also a(n) = A000005(A283477(n)).
 * https://oeis.org/A284005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (1 + popcount(n)) * a(floor(n/2)) for n > 1 a(0) = 1,
 *            a(1) = 2 Uses memoisation for arbitrary‑precision or integral
 *            seq_t types
 * Range:     u8 n <= 25, u16 n <= 189, u32 n <= 4086, u64 n <= 8191,
 *            u128 n <= 3, big any n
 * Helpers:   _popcount_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_popcount_u64.h"

/* keep a cache of already computed values */
static seq_t *cache = NULL;      /* array of seq_t, indexed by n */
static size_t cache_len = 0;     /* number of allocated slots      */

/* Ensure the cache can hold index n (i.e. indices 0..n inclusive) */
static void _ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_len)
        return;

    size_t new_len = cache_len ? cache_len : 1;
    while (new_len < needed)
        new_len <<= 1;           /* grow by powers of two */

    /* reallocate; cast for C++ compatibility */
    seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort();                 /* allocation failure */

    cache = new_cache;
    cache_len = new_len;

    /* initialise new entries to zero (or default constructed seq_t) */
    for (size_t i = needed; i < new_len; ++i)
        cache[i] = (seq_t)0;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    _ensure_cache(n);
    if (cache[n] != (seq_t)0)
        return cache[n];

    idx_t half = n >> 1;           /* floor(n/2) */
    seq_t sub  = a(half);
    unsigned int factor = 1 + _popcount_u64(n);
    seq_t result = (seq_t)factor * sub;

    cache[n] = result;             /* memoise */
    return result;
}
