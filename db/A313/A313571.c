/*
 * A313571  Coordination sequence Gal.5.169.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A313571 – Coordination sequence Gal.5.169.3
 * Recurrence: a(n) = 2*a(n-7) - a(n-14) for n > 17
 * with initial values a(0)..a(17) as given.
 */

#include <stdlib.h>

/* initial values a(0)..a(17) */
static const seq_t init[18] = {
    1ULL, 5ULL, 10ULL, 14ULL, 19ULL, 24ULL, 29ULL, 35ULL,
    39ULL, 46ULL, 48ULL, 52ULL, 60ULL, 62ULL, 71ULL, 72ULL,
    82ULL, 81ULL
};

static seq_t *cache      = NULL;   /* cache holding a(0)..a(max) */
static size_t cache_cap  = 0;       /* current capacity of cache */
static size_t computed_to = 0;      /* number of elements already computed */

/* ensure cache has space for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < (idx_t)cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap : 18;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    /* realloc is C++-safe because of explicit cast */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return; /* allocation failure; let caller handle */
    cache = tmp;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* use the given initial values for n < 18 */
    if (n < 18) return init[(size_t)n];

    /* first time we need values beyond the initial block – allocate cache */
    if (!cache) {
        cache = (seq_t *)malloc(18 * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure, fall back */
        for (size_t i = 0; i < 18; ++i) cache[i] = init[i];
        cache_cap   = 18;
        computed_to = 18;
    }

    ensure_capacity(n);

    /* compute missing values up to n */
    if (computed_to <= (size_t)n) {
        for (size_t i = computed_to; i <= (size_t)n; ++i) {
            cache[i] = cache[i-7] * (seq_t)2 - cache[i-14];
        }
        computed_to = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
