/*
 * A315047  Coordination sequence Gal.5.204.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -a(n-1) - a(n-2) + 2*a(n-5) + 2*a(n-6) + 2*a(n-7) -
 *            a(n-10) - a(n-11) - a(n-12) (n > 12) Initial values: a(0)=1
 *            a(1)=5 a(2)=9 a(3)=15 a(4)=20 a(5)=24 a(6)=28 a(7)=35 a(8)=39
 *            a(9)=43 a(10)=50 a(11)=51 a(12)=59
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoised values */
static seq_t *cache       = NULL;      /* array of a(0)..a(max)            */
static size_t cache_cap   = 0;         /* allocated capacity               */
static idx_t  cache_max      = (idx_t)(-1); /* highest stored index (invalid) */

/* First thirteen terms, indices 0..12 */
static const seq_t init_vals[13] = {
    1, 5, 9, 15, 20, 24, 28, 35, 39, 43, 50, 51, 59
};

/* Ensure the cache has capacity at least 'need' (need >= cache_cap). */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;

    size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
    while (new_cap < need) new_cap <<= 1;           /* double until enough */

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (new_cache == NULL) abort();                /* allocation failed */

    /* Copy old values, if any. */
    if (cache != NULL) {
        for (idx_t i = 0; i <= cache_max; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
    }

    cache     = new_cache;
    cache_cap = new_cap;
}

/* Main function computing a(n). */
seq_t a(idx_t n)
{
    /* If the array is not yet allocated, make room for at least 13 entries. */
    if (cache == NULL) {
        ensure_cache_capacity(13);
        /* Initialize the first thirteen terms. */
        for (idx_t i = 0; i < 13; ++i) cache[i] = init_vals[i];
        cache_max = 12;
    }

    /* If n is already cached, return it. */
    if (n <= (idx_t)cache_max) return cache[n];

    /* Extend the cache to include n. */
    ensure_cache_capacity((size_t)n + 1);

    /* Compute missing values up to n using the recurrence. */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t val;
        if (k <= 12) {                 /* safety: fill any missing 0..12 terms */
            val = init_vals[k];
        } else {
            /* Recurrence: (2*a(k-5)+...+2*a(k-7)) - (a(k-1)+a(k-2)+a(k-10)+a(k-11)+a(k-12)) */
            seq_t part1 = (cache[k-5] << 1) + (cache[k-6] << 1) + (cache[k-7] << 1);
            seq_t part2 = cache[k-1] + cache[k-2] + cache[k-10] + cache[k-11] + cache[k-12];
            val = part1 - part2;                  /* Guaranteed non‑negative */
        }
        cache[k] = val;
    }

    cache_max = n;
    return cache[n];
}
