/*
 * A299263  Partial sums of A299257.
 * https://oeis.org/A299263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A299263 – Partial sums of A299257
   Recurrence (valid for n > 9):
   a(n) = 4*a(n-1) - 8*a(n-2) + 12*a(n-3) - 14*a(n-4)
          + 12*a(n-5) - 8*a(n-6) + 4*a(n-7) - a(n-8)
   Initial values:
   a(0)=1, a(1)=6, a(2)=18, a(3)=40, a(4)=76,
   a(5)=132, a(6)=214, a(7)=325, a(8)=469, a(9)=652
   The function computes any a(n) for n ≥ 0 and memoises results
   for fast subsequent access. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* allocated array of terms    */
static size_t cache_count   = 0;     /* number of terms that are valid */
static size_t cache_capacity = 0;    /* number of elements allocated */

/* Allocate the first 10 terms (defined above). */
static void init_cache(void)
{
    cache_capacity = 10;
    cache_count    = cache_capacity;
    cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)6;
    cache[2] = (seq_t)18;
    cache[3] = (seq_t)40;
    cache[4] = (seq_t)76;
    cache[5] = (seq_t)132;
    cache[6] = (seq_t)214;
    cache[7] = (seq_t)325;
    cache[8] = (seq_t)469;
    cache[9] = (seq_t)652;
}

/* Grow the allocated array so that indices up to n are valid. */
static void ensure_capacity(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (cache_capacity >= required)
        return;

    size_t new_capacity = cache_capacity > 0 ? cache_capacity : 10;
    while (new_capacity < required)
        new_capacity <<= 1;        /* exponential growth */

    seq_t *new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    cache = new_cache;
    cache_capacity = new_capacity;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (cache == NULL)
        init_cache();

    /* Return cached value if already computed. */
    if ((size_t)n < cache_count)
        return cache[(size_t)n];

    /* Ensure we have storage for all terms up to n. */
    ensure_capacity(n);

    /* Compute missing terms using the 8‑term recurrence. */
    for (idx_t i = (idx_t)cache_count; i <= n; ++i)
    {
        /* All indices accessed are < i, so already initialized. */
        seq_t v = (seq_t)4 * cache[(size_t)i - 1]
                - (seq_t)8 * cache[(size_t)i - 2]
                + (seq_t)12 * cache[(size_t)i - 3]
                - (seq_t)14 * cache[(size_t)i - 4]
                + (seq_t)12 * cache[(size_t)i - 5]
                - (seq_t)8  * cache[(size_t)i - 6]
                + (seq_t)4  * cache[(size_t)i - 7]
                - cache[(size_t)i - 8];
        cache[(size_t)i] = v;
    }

    cache_count = (size_t)n + 1;    /* update number of valid terms */
    return cache[(size_t)n];
}
