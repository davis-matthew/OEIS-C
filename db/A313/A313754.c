/*
 * A313754  Coordination sequence Gal.4.142.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n > 10, a(n) = 2*a(n-1) - a(n-2) - a(n-4) + 2*a(n-5) -
 *            a(n-6) - a(n-8) + 2*a(n-9) - a(n-10) Initial values: a(0)=1,
 *            a(1)=5, a(2)=10, a(3)=15, a(4)=20, a(5)=26, a(6)=32, a(7)=38,
 *            a(8)=44, a(9)=49, a(10)=54
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic array of computed values */
static size_t cache_len = 0;     /* allocated length of cache */
static idx_t  max_computed = (idx_t)-1;  /* greatest index for which cache is set */

/* initial values for the first 11 terms */
static const seq_t init_vals[11] = {
    1ULL, 5ULL, 10ULL, 15ULL, 20ULL,
    26ULL, 32ULL, 38ULL, 44ULL, 49ULL, 54ULL
};

/* initialize cache with the first 11 values */
static void init_cache(void)
{
    cache_len = 11;
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    if (!cache)
        abort();
    for (size_t i = 0; i < cache_len; ++i)
        cache[i] = init_vals[i];
    max_computed = 10;
}

/* enlarge the cache to hold at least (n+1) elements */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed > cache_len) {
        size_t new_len = needed;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)
            abort();
        cache = new_cache;
        cache_len = new_len;
    }
}

/* public function */
seq_t a(idx_t n)
{
    if (!cache)               /* first call – initialise cache */
        init_cache();

    if (n <= max_computed)
        return cache[(size_t)n];

    ensure_cache_size(n);

    /* compute missing terms using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* pos = 2*a(i-1) + 2*a(i-5) + 2*a(i-9) */
        seq_t pos = cache[(size_t)(i - 1)] * 2
                  + cache[(size_t)(i - 5)] * 2
                  + cache[(size_t)(i - 9)] * 2;

        /* neg = a(i-2) + a(i-4) + a(i-6) + a(i-8) + a(i-10) */
        seq_t neg = cache[(size_t)(i - 2)]
                  + cache[(size_t)(i - 4)]
                  + cache[(size_t)(i - 6)]
                  + cache[(size_t)(i - 8)]
                  + cache[(size_t)(i - 10)];

        cache[(size_t)i] = pos - neg;
    }

    max_computed = n;
    return cache[(size_t)n];
}
