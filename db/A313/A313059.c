/*
 * A313059  Coordination sequence Gal.4.99.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (Conjecture from OEIS A313059):
   a(0..10) = {1,4,9,14,18,23,29,32,37,44,45}
   for n > 10:
   a(n) = a(n-1) - a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5)
          - a(n-6) + a(n-7) - a(n-8) */

/* <-- The code below is valid C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>

/* External integer types provided by the build system. */
/* (No redefinitions are made here.) */

static seq_t *cache     = NULL;  /* cache of computed a(n) values      */
static size_t cache_len = 0;     /* number of values stored            */
static size_t cache_cap = 0;     /* current allocation capacity        */

/* Ensure the cache can hold at least 'need' elements (0‑based). */
static void ensure_cache_cap(idx_t need)
{
    if ((idx_t)cache_cap > need)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while ((idx_t)new_cap <= need)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* Allocation failure */
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Initialize cache with the 11 known base terms. */
    if (cache == NULL)
    {
        /* Allocate enough space for the base terms and a few extra. */
        ensure_cache_cap(10);
        static const seq_t base[11] = {
            1ull, 4ull, 9ull, 14ull, 18ull, 23ull, 29ull, 32ull,
            37ull, 44ull, 45ull
        };
        for (idx_t i = 0; i <= 10; ++i)
            cache[i] = base[i];
        cache_len = 11;          /* a(0)…a(10) are stored       */
    }

    /* If the requested term is already cached return it. */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache up to the required index. */
    ensure_cache_cap(n);
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* The recurrence is valid for i > 10; for <=10 the base values
           already exist. */
        if (i <= 10)
        {
            /* already set via the base array */
            continue;
        }
        else
        {
            seq_t val =   cache[i-1]
                         - cache[i-2]
                         + 2 * cache[i-3]
                         - 2 * cache[i-4]
                         + 2 * cache[i-5]
                         - cache[i-6]
                         + cache[i-7]
                         - cache[i-8];
            cache[i] = val;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
