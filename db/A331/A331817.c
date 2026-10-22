/*
 * A331817  a(n) = (n!)^2 * Sum_{k=0..n} (2*k)! / (2^k * (k!)^3 * (n - k)!).
 * https://oeis.org/A331817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 2,
 * a(n+2) = 2*(3+2*n)*a(n+1) - 3*(n+1)^2*a(n)  (OEIS A331817) */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values a(0)..a(n)   */
static size_t cache_cap = 0;     /* allocated array capacity   */
static idx_t cache_len = 0;      /* number of computed entries  */

/* Ensure a(0)..a(n) are computed */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;               /* already done */

    idx_t needed = n + 1;                     /* indices 0..n                */
    if (needed > cache_cap) {                /* grow array if required       */
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first entries if needed */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;                 /* a(0) */
        cache_len = 1;
    }
    if (cache_len == 1 && needed > 1) {
        cache[1] = (seq_t)2;                 /* a(1) */
        cache_len = 2;
    }

    /* compute the remaining terms up to n   */
    for (idx_t i = cache_len; i <= n; ++i) {
        idx_t prev1 = i - 1;                 /* a(i-1) */
        idx_t prev2 = i - 2;                 /* a(i-2) */

        /* coefficient = 3 + 2*prev2  (as seq_t) */
        seq_t coef = (seq_t)3 + (seq_t)2 * (seq_t)prev2;
        /* term1 = 2 * coef * a(i-1) */
        seq_t term1 = (seq_t)2 * coef * cache[prev1];

        /* sq = (prev1)^2  */
        seq_t sq = (seq_t)prev1;
        sq = sq * sq;
        /* term2 = 3 * sq * a(i-2) */
        seq_t term2 = (seq_t)3 * sq * cache[prev2];

        cache[i] = term1 - term2;
    }

    cache_len = needed;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
