/*
 * A314024  Coordination sequence Gal.6.156.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5, a(2)=11, a(3)=15, a(4)=20, a(5)=25, a(6)=29,
 *            a(7)=33, a(8)=38, a(9)=43, a(10)=47; a(n) =
 *            a(n-1)+a(n-3)-a(n-4)-a(n-6)+a(n-7)+a(n-9)-a(n-10) for n>10
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* Initialise the cache with the first 11 values. */
static void init_cache(void)
{
    const size_t init_cap = 16;
    cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    cache_cap = init_cap;

    cache[0] = 1;
    cache[1] = 5;
    cache[2] = 11;
    cache[3] = 15;
    cache[4] = 20;
    cache[5] = 25;
    cache[6] = 29;
    cache[7] = 33;
    cache[8] = 38;
    cache[9] = 43;
    cache[10] = 47;

    cache_size = 11;   /* elements 0..10 are now valid */
}

/* Extend the cache so that it contains all values up to `up_to`. */
static void ensure_cache(size_t up_to)
{
    /* If the cache already contains the required values, return immediately. */
    if (up_to < cache_size) return;

    /* Allocate the cache on first use. */
    if (!cache) init_cache();

    /* Grow the capacity if necessary. */
    if (up_to + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= up_to) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values using the recurrence. */
    for (size_t i = cache_size; i <= up_to; ++i) {
        /* For i <= 10 the values are already initialised. */
        if (i <= 10) continue;

        seq_t a1 = cache[i - 1];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a6 = cache[i - 6];
        seq_t a7 = cache[i - 7];
        seq_t a9 = cache[i - 9];
        seq_t a10 = cache[i - 10];

        seq_t pos = a1 + a3;
        pos += a7;
        pos += a9;

        seq_t neg = a4 + a6;
        neg += a10;

        cache[i] = pos - neg;
    }

    cache_size = up_to + 1;
}

/* Public routine required by the tests. */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_cache(idx);
    return cache[idx];
}
