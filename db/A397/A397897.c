/*
 * A397897  a(n) = Sum_{j=1..n} (-1)^(j+1) * j * prime(n-j+1).
 * https://oeis.org/A397897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 34, i16 n <= 1157, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A397897
   Formula: a(n) = prime(n) - 2*a(n-1) - a(n-2)   for n > 1
   Initial values: a(0) = 0, a(1) = 2
   (prime(k) is the k‑th prime, 1‑indexed: prime(1)=2) */

#include <stdlib.h>
#include "../utils/get_prime.h"

static seq_t *cache = NULL;   /* memoisation array */
static size_t cache_cap = 0;  /* allocated size of cache */
static size_t cache_len = 0;  /* number of values already stored */

/* enlarge the cache so that it can store at least 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed;
    if (new_cap < 2)
        new_cap = 2;
    if (new_cap < cache_cap * 2)
        new_cap = cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    size_t nn = (size_t)n;

    /* make sure the cache can hold the requested index */
    ensure_cache_cap(nn + 1);

    /* if the value is already cached, just return it */
    if (nn < cache_len)
        return cache[nn];

    /* initialise the first terms if they are missing */
    if (cache_len == 0) {
        cache[0] = 0;
        cache_len = 1;
    }
    if (nn == 0)
        return cache[0];

    if (cache_len == 1) {
        cache[1] = 2;
        cache_len = 2;
    }
    if (nn == 1)
        return cache[1];

    /* compute all missing terms up to nn */
    for (size_t i = cache_len; i <= nn; ++i) {
        seq_t prime_val = (seq_t)get_prime((idx_t)i);   /* i is 1‑indexed */
        seq_t prev = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = prime_val - 2 * prev - prev2;
    }
    cache_len = nn + 1;
    return cache[nn];
}
