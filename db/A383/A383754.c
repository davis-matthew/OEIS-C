/*
 * A383754  Expansion of 1/Product_{k=0..3} (1 - 2^k * 3^(3-k) * x).
 * https://oeis.org/A383754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 65*a(n-1) - 1482*a(n-2) + 14040*a(n-3) - 46656*a(n-4);
 *            Initial values: a(0)=1, a(1)=65, a(2)=2743, a(3)=96005
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;          /* cache of computed terms */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* current cache capacity (elements) */

/* Computes a(i) for i >= 4 using the recurrence */
static seq_t compute_next(idx_t i)
{
    seq_t t = mul_small(cache[i-1], 65);   /* 65 * a(i-1) */
    t -= mul_small(cache[i-2], 1482);      /* -1482 * a(i-2) */
    t += mul_small(cache[i-3], 14040);     /* +14040 * a(i-3) */
    t -= mul_small(cache[i-4], 46656);     /* -46656 * a(i-4) */
    return t;
}

/* Ensures that cache contains at least n+1 elements (index n exists) */
static void ensure_cache(idx_t n)
{
    /* Initialise minimal cache if empty */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)65;
        cache[2] = (seq_t)2743;
        cache[3] = (seq_t)96005;
        cache_len = 4;
    }

    /* Grow cache capacity if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    while (cache_len <= n) {
        cache[cache_len] = compute_next(cache_len);
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
