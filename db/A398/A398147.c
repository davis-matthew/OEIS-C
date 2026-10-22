/*
 * A398147  Expansion of generating function x/(1-5*x+x^2)^2.
 * https://oeis.org/A398147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 27*a(n-2) + 10*a(n-3) - a(n-4) for n >= 5;
 *            a(1)=1, a(2)=10, a(3)=73, a(4)=470
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

/* Allocate the cache for the four initial values. */
static void init_cache(void)
{
    size_t cap = 5;                 /* indices 0..4 (0 unused) */
    cache = (seq_t*)calloc(cap, sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);

    cache[1] = (seq_t)1;
    cache[2] = (seq_t)10;
    cache[3] = (seq_t)73;
    cache[4] = (seq_t)470;

    cache_cap = cap;
    cache_max = 4;
}

/* Make sure the cache can hold indices up to n (1‑based). */
static void grow_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_cap)
        return;

    seq_t *new_cache = (seq_t*)realloc(cache, required * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);

    /* Zero the newly allocated slots. */
    memset(new_cache + cache_cap, 0,
           (required - cache_cap) * sizeof(seq_t));

    cache = new_cache;
    cache_cap = required;
}

/* Extend the cache up to index n using the linear recurrence. */
static void compute_up_to(idx_t n)
{
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        seq_t term = (seq_t)10 * cache[i-1]
                   - (seq_t)27 * cache[i-2]
                   + (seq_t)10 * cache[i-3]
                   -            cache[i-4];
        cache[i] = term;
    }
    cache_max = n;
}

/* Public function: a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    if (!cache)
        init_cache();

    if (n <= cache_max)
        return cache[n];

    grow_cache(n);
    compute_up_to(n);
    return cache[n];
}
