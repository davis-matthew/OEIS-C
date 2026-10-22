/*
 * A392430  a(n) = Sum_{k=0..floor(2*n/7)} binomial(2*k,2*n-7*k).
 * https://oeis.org/A392430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 38, u32 n <= 69, u64 n <= 133,
 *            u128 n <= 260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-4) + a(n-7) - 2*a(n-8) + a(n-9)  (n ≥ 9)
 * Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=0,
 *                 a(4)=2, a(5)=0, a(6)=0, a(7)=1, a(8)=6
 */
#include <stddef.h>
#include <stdlib.h>

/* Dynamic cache for computed terms */
static seq_t *cache  = NULL;   /* pointer to cache array */
static size_t cache_len = 0;   /* number of elements currently stored */
static size_t cache_cap = 0;   /* allocated capacity of the array */

/* Ensure that cache holds entries up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* initialise cache with the first 9 terms */
    if (cache == NULL) {
        cache_cap = 9;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) abort();
        cache[0] = 1;  /* a(0) */
        cache[1] = 0;  /* a(1) */
        cache[2] = 0;  /* a(2) */
        cache[3] = 0;  /* a(3) */
        cache[4] = 2;  /* a(4) */
        cache[5] = 0;  /* a(5) */
        cache[6] = 0;  /* a(6) */
        cache[7] = 1;  /* a(7) */
        cache[8] = 6;  /* a(8) */
        cache_len = 9;
    }

    /* already computed up to n? */
    if ((size_t)cache_len > (size_t)n)
        return;

    /* grow the array if needed */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap *= 2;   /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence is valid for i ≥ 9; i starts at cache_len ≥ 9 */
        seq_t t = 4 * cache[(size_t)(i - 4)] + cache[(size_t)(i - 7)] + cache[(size_t)(i - 9)];
        cache[(size_t)i] = t - 2 * cache[(size_t)(i - 8)];
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
