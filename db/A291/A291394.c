/*
 * A291394  p-INVERT of (1,1,0,0,0,0,...), where p(S) = (1 - S)(1 - 3 S).
 * https://oeis.org/A291394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 6*a(n-3) - 3*a(n-4) (n >= 5) a(0) =
 *            4, a(1) = 17, a(2) = 66, a(3) = 254, a(4) = 968
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* cached sequence values */
static seq_t *cache       = NULL;
static size_t cache_size  = 0;
/* highest index that has been computed, (size_t)-1 means “none” yet */
static size_t cached_up_to = (size_t)-1;

static const seq_t base_vals[5] = { (seq_t)4, (seq_t)17, (seq_t)66, (seq_t)254, (seq_t)968 };

/* Grow the cache so that it can hold at least indices 0 … n */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size)
        return;
    size_t new_cap = cache_size ? cache_size * 2 : 8;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_size = new_cap;
}

/* Return the nth term of sequence A291394 */
seq_t a(idx_t n)
{
    /* base values – no caching required */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)17;
    if (n == 2) return (seq_t)66;
    if (n == 3) return (seq_t)254;
    if (n == 4) return (seq_t)968;

    /* Ensure the cache can hold the requested element */
    ensure_cache(n);

    /* Initialise cache with the first five values the first time we need them */
    if (cached_up_to == (size_t)-1) {
        for (size_t i = 0; i < 5; ++i)
            cache[i] = base_vals[i];
        cached_up_to = 4;
    }

    /* If we already know this value, return it */
    if ((size_t)n <= cached_up_to)
        return cache[(size_t)n];

    /* Compute all terms from the next unknown index up to n */
    for (size_t i = cached_up_to + 1; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        cache[i] = 4 * a1 + a2 - 6 * a3 - 3 * a4;
    }

    cached_up_to = (size_t)n;
    return cache[(size_t)n];
}
