/*
 * A289918  p-INVERT of (1,0,0,1,0,0,1,0,0,...), where p(S) = (1 - S^2).
 * https://oeis.org/A289918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3) - a(n-6) for n ≥ 6; a(0) = 0, a(1) = 1,
 *            a(2) = 0, a(3) = 1, a(4) = 2, a(5) = 1
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 61, u64 n <= 119,
 *            u128 n <= 235, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t capacity = 0;
static idx_t cached_size = 0; /* number of values currently stored */

/* Ensure the cache is allocated and the first six terms are set. */
static void init_cache(idx_t needed)
{
    if (cache == NULL) {
        /* Minimum size is six entries for the recurrence */
        capacity = needed + 1;
        if (capacity < 6) capacity = 6;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!cache) abort();
        /* Initial values of the sequence */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)1;
        cached_size = 6;
    }
}

/* Grow the cache to at least the requested size. */
static void ensure_capacity(idx_t needed)
{
    if (needed + 1 <= capacity) return;
    size_t new_cap = capacity ? capacity * 2 : 1;
    while (new_cap < needed + 1) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    capacity = new_cap;
}

/* Main sequence function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (!cache) init_cache(n);
    else ensure_capacity(n);

    if (n < cached_size) return cache[n];

    /* Compute terms up to n using the given recurrence. */
    for (idx_t i = cached_size; i <= n; ++i) {
        seq_t val = cache[i - 2] + 2 * cache[i - 3] - cache[i - 6];
        cache[i] = val;
    }
    cached_size = n + 1;
    return cache[n];
}
