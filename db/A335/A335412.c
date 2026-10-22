/*
 * A335412  a(n) is the number of edges formed by n-secting the angles of an
 *          equilateral triangle.
 * https://oeis.org/A335412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-10) - a(n-11) - a(n-12) +
 *            a(n-13); initial values: a(1)=3, a(2)=12, a(3)=39, a(4)=54,
 *            a(5)=123, a(6)=144, a(7)=255, a(8)=282, a(9)=435, a(10)=432,
 *            a(11)=663, a(12)=702, a(13)=939
 * Range:     u8 n <= 7, u16 n <= 105, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* 1‑based cache, cache[0] unused */
static size_t cache_cap = 0;     /* number of allocated elements */
static bool initialized = false;

/* Populate the cache with the first 13 terms. */
static void init_cache(void)
{
    /* Allocate space for indices 0..13 (14 elements). */
    cache = (seq_t *)malloc(14 * sizeof(seq_t));
    if (!cache) abort();  /* fatal if allocation fails */
    cache_cap = 14;
    cache[0] = (seq_t)0;            /* unused */
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)12;
    cache[3] = (seq_t)39;
    cache[4] = (seq_t)54;
    cache[5] = (seq_t)123;
    cache[6] = (seq_t)144;
    cache[7] = (seq_t)255;
    cache[8] = (seq_t)282;
    cache[9] = (seq_t)435;
    cache[10] = (seq_t)432;
    cache[11] = (seq_t)663;
    cache[12] = (seq_t)702;
    cache[13] = (seq_t)939;
    initialized = true;
}

/* Ensure that cache has entries up to index n, computing new values as needed. */
static void extend_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;  /* already big enough */

    /* Grow the array to size n+1. */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* fatal if allocation fails */
    cache = tmp;

    /* Compute values for indices from old capacity to n. */
    for (idx_t m = (idx_t)cache_cap; m <= n; ++m)
    {
        /* Guarantee that m > 13 for the recurrence to be valid. */
        /* For m <= 13 the values were already initialized. */
        seq_t v = cache[(size_t)(m - 1)] + cache[(size_t)(m - 2)] - cache[(size_t)(m - 3)]
                 + cache[(size_t)(m - 10)] - cache[(size_t)(m - 11)]
                 - cache[(size_t)(m - 12)] + cache[(size_t)(m - 13)];
        cache[(size_t)m] = v;
    }
    cache_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;  /* undefined for n<1, return 0 */

    if (!initialized) init_cache();
    if ((idx_t)cache_cap <= n) extend_cache(n);
    return cache[(size_t)n];
}
