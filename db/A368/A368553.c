/*
 * A368553  a(n) = Sum_{k=0..n} k^k * 2^(n-k).
 * https://oeis.org/A368553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)=2*a(n-1)+n^n; */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp); */

static seq_t *cache = NULL;          /* holds computed a(0)..a(k) */
static idx_t  cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity of cache */

/* Allocate or grow the cache to hold at least cap elements. */
static void ensure_cache_cap(idx_t cap)
{
    if ((size_t)cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < (size_t)cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)          /* allocation failed */
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    /* initialise a(0) on first call */
    if (cache_len == 0) {
        ensure_cache_cap(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* compute missing terms up to n */
    ensure_cache_cap(n + 1);
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev    = cache[i - 1];
        seq_t ipow    = big_pow((seq_t)i, (seq_t)i);   /* i^i */
        seq_t two_prev = prev * (seq_t)2;              /* 2 * a(i-1) */
        cache[i] = two_prev + ipow;
    }
    cache_len = n + 1;
    return cache[n];
}
