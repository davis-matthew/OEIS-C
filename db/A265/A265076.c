/*
 * A265076  Coordination sequence for (3,5,5) tiling of hyperbolic plane.
 * https://oeis.org/A265076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-3) + a(n-5) - a(n-6) Initial values:
 *            a(0)=1, a(1)=3, a(2)=6, a(3)=11, a(4)=20, a(5)=35, a(6)=60
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 39, u64 n <= 80, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base[7] = {1, 3, 6, 11, 20, 35, 60};

static seq_t *cache = NULL;     /* dynamic array storing computed values */
static idx_t cache_len = 0;     /* number of initialized entries in cache   */
static size_t cache_cap = 0;    /* current capacity of the cache array       */

seq_t a(idx_t n)
{
    /* n less than the precomputed range – return immediately */
    if (n < 7) return base[n];

    /* First call: allocate memory and fill in known values */
    if (cache_len == 0) {
        size_t init_cap = (n + 1 > 7 ? n + 1 : 7);
        cache = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (!cache) return 0; /* out of memory – return 0 as a safe fallback */
        for (idx_t i = 0; i < 7; ++i) cache[(size_t)i] = base[i];
        cache_len = 7;
        cache_cap = init_cap;
    }

    /* Grow cache if necessary   */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – safe fallback */
        cache = tmp;
        /* Newly allocated memory is left uninitialized; we will fill it.   */
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = cache[(size_t)(i - 1)]
                  + 2 * cache[(size_t)(i - 3)]
                  + cache[(size_t)(i - 5)]
                  - cache[(size_t)(i - 6)];
        cache[(size_t)i] = val;
    }
    if (n + 1 > cache_len) cache_len = n + 1;

    return cache[(size_t)n];
}
