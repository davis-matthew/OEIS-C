/*
 * A247919  Expansion of 1 / (1 + x^4 - x^5) in powers of x.
 * https://oeis.org/A247919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 43, i16 n <= 79, i32 n <= 159, i64 n <= 316,
 *            i128 n <= 632, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A247919 – Expansion of 1 / (1 + x^4 - x^5)
    Recurrence:  a(n) = a(n-5) - a(n-4)   for n ≥ 5
    Initial values:  a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=-1
*/

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;   /* number of valid entries in cache   */
static idx_t  cache_cap = 0;   /* capacity of cache array            */

static void ensure_cache(idx_t n)
{
    /* make sure cache has entries 0 .. n */
    if (cache_len > 0) {
        if (n < cache_len)
            return;   /* already have it */
    } else {
        /* initialize base cases */
        if (n < 5) {
            cache_cap = 5;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) abort();
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)0;
            cache[3] = (seq_t)0;
            cache[4] = (seq_t)-1;
            cache_len = 5;
        } else {
            cache_cap = n + 1;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) abort();
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)0;
            cache[3] = (seq_t)0;
            cache[4] = (seq_t)-1;
            cache_len = 5;
        }
    }

    if (n < cache_len)
        return;

    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) {
            new_cap *= 2;
        }
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-5] - cache[i-4];
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
