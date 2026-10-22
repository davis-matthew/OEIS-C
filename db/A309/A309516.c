/*
 * A309516  Number of odd parts in the partitions of n into 4 parts.
 * https://oeis.org/A309516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309516: Number of odd parts in the partitions of n into 4 parts.
   Computation uses the recurrence:
   a(n) = 2*a(n-1) - a(n-2) + a(n-6) - 2*a(n-7) + 2*a(n-8) - 2*a(n-9)
          + a(n-10) - a(n-14) + 2*a(n-15) - a(n-16)   for n>15
   with initial values:
   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=4, a(5)=3, a(6)=6, a(7)=7,
   a(8)=12, a(9)=14, a(10)=22, a(11)=25, a(12)=36, a(13)=40,
   a(14)=52, a(15)=59. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;
    static idx_t cache_size = 0;          /* number of already computed entries */

    /* Initial values for n = 0..15 */
    static const seq_t base[16] = {
        0ull, 0ull, 0ull, 0ull, 4ull, 3ull,
        6ull, 7ull, 12ull, 14ull, 22ull, 25ull,
        36ull, 40ull, 52ull, 59ull
    };

    /* Allocate and initialise the cache if needed */
    if (!cache) {
        /* start with a modest capacity, at least 16 entries */
        cache_capacity = 128;
        if (cache_capacity < 16)
            cache_capacity = 16;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) abort();                /* memory allocation failed */
        for (idx_t i = 0; i < 16; ++i) cache[i] = base[i];
        cache_size = 16;                    /* we have computed indices 0..15 */
    }

    /* If the requested n is already computed, return it */
    if (n < cache_size)
        return cache[n];

    /* Ensure the cache is large enough to hold up to index n */
    if (n + 1 > cache_capacity) {
        idx_t new_cap = cache_capacity ? cache_capacity * 2 : 128;
        while (new_cap <= n) new_cap <<= 1;   /* grow until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute all values from cache_size up to n inclusive */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 16) {                /* safeguard, should already be initialised */
            cache[i] = base[i];
        } else {                     /* use the recurrence */
            seq_t sum_pos = 2 * cache[i - 1];
            sum_pos += cache[i - 6];
            sum_pos += 2 * cache[i - 8];
            sum_pos += cache[i - 10];
            sum_pos += 2 * cache[i - 15];

            seq_t sum_neg = cache[i - 2];
            sum_neg += 2 * cache[i - 7];
            sum_neg += 2 * cache[i - 9];
            sum_neg += cache[i - 14];
            sum_neg += cache[i - 16];

            cache[i] = sum_pos - sum_neg;
        }
    }
    cache_size = n + 1;
    return cache[n];
}
