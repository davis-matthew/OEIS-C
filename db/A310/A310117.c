/*
 * A310117  Coordination sequence Gal.5.20.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310117
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

/* Sequence: A310117 – Coordination sequence Gal.5.20.1
   Recurrence:
      a(0)=1,  a(1)=3,  a(2)=6,  a(3)=11,  a(4)=18,  a(5)=21,  a(6)=23,
      a(7)=26, a(8)=29, a(9)=32, a(10)=39, a(11)=48, a(12)=49, a(13)=49,
      a(14)=52;
      for n>14:  a(n) = 2*a(n-7) - a(n-14)  */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static const seq_t init_vals[15] = {
    1, 3, 6, 11, 18, 21, 23, 26, 29, 32, 39, 48, 49, 49, 52
};

/* Cache for computed values.  We grow it dynamically as needed. */
static seq_t *cache = NULL;          /* pointer to allocated array      */
static idx_t  cache_len = 0;         /* number of elements currently stored */
static bool   initialized = false;   /* true once init_vals copied into cache */

/* Allocate or resize the cache to hold at least (n+1) entries.  Returns
   true on success, false if malloc/realloc fails.  The cache is
   initialized with init_vals for indices < 15.  */
static bool
ensure_cache(idx_t n)
{
    if (!initialized) {
        /* First allocation and copy initial values. */
        size_t cap = 15;
        if (n + 1 > cap) cap = n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return false;
        for (idx_t i = 0; i < 15; ++i) cache[i] = init_vals[i];
        cache_len = cap;
        initialized = true;
    }

    if (n + 1 <= cache_len) return true;   /* already big enough */

    /* Grow the cache.  double the size until it fits. */
    size_t new_cap = cache_len * 2;
    if (new_cap < n + 1) new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return false;   /* allocation failure */
    cache = tmp;
    cache_len = new_cap;
    return true;
}

seq_t a(idx_t n)
{
    /* Return the initial segment directly if possible. */
    if (n < 15) return init_vals[n];

    /* Ensure we have room to compute up to n. */
    if (!ensure_cache(n)) return 0;   /* unlikely in the tests */

    /* Compute missing terms only once. */
    for (idx_t i = 15; i <= n; ++i) {
        /* Use a local variable for 2 to avoid type‑cast issues with
           arbitrary‑precision seq_t. */
        seq_t two = 2;
        cache[i] = two * cache[i - 7] - cache[i - 14];
    }
    return cache[n];
}
