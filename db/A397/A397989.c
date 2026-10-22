/*
 * A397989  Upper (1/3,1/2) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A397989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-6) - 2*a(n-7) + a(n-8), with
 *            (a(0),...,a(7)) = (0,1,3,7,12,18,25,34)
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[8] = {0, 1, 3, 7, 12, 18, 25, 34};
static seq_t  *cache = NULL;
static size_t cache_cap = 0; /* number of allocated slots, always >= cache_len */
static size_t cache_len = 0; /* number of initialized elements */

/* Ensure cache holds 0..n.  After calling, cache_len >= n+1. */
static void ensure_cache(idx_t n)
{
    /* Small n: nothing to do, we will return from init_vals. */
    if (n < 8) return;

    /* Allocate or grow cache if needed */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
        if (cache_cap < 8) cache_cap = 8;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        for (size_t i = 0; i < 8; ++i) cache[i] = init_vals[i];
        cache_len = 8;
    } else if (cache_cap <= (size_t)n) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
    }

    /* Compute new values up to n */
    for (; cache_len <= (size_t)n; ++cache_len) {
        idx_t k = cache_len;
        seq_t a1 = cache[(size_t)(k - 1)];
        seq_t a2 = cache[(size_t)(k - 2)];
        seq_t a6 = cache[(size_t)(k - 6)];
        seq_t a7 = cache[(size_t)(k - 7)];
        seq_t a8 = cache[(size_t)(k - 8)];

        /* Evaluate recurrence carefully to avoid underflow in unsigned arithmetic */
        seq_t val = 2 * a1;          /* 2*a(k-1) */
        val -= a2;                   /* - a(k-2) */
        val += a6;                   /* + a(k-6) */
        val -= 2 * a7;               /* - 2*a(k-7) */
        val += a8;                   /* + a(k-8) */

        cache[(size_t)k] = val;
    }
}

seq_t a(idx_t n)
{
    if (n < 8) return init_vals[(size_t)n];

    ensure_cache(n);
    return cache[(size_t)n];
}
