/*
 * A312969  Coordination sequence Gal.6.254.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 3*a(n-3) - a(n-4) - a(n-6) +
 *            3*a(n-7) - 4*a(n-8) + 3*a(n-9) - a(n-10) (for n > 10) Initial
 *            values: a(0)=1, a(1)=4, a(2)=9, a(3)=13, a(4)=19, a(5)=23,
 *            a(6)=27, a(7)=31, a(8)=35, a(9)=41, a(10)=45
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define INITIAL_CAP 11

static const seq_t initial[11] = {1,4,9,13,19,23,27,31,35,41,45};

static seq_t *cache = NULL;      /* dynamically grown cache */
static size_t cache_cap = 0;      /* allocated size of cache */

/* helper to ensure cache has room for up to n */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : INITIAL_CAP;
    while (new_cap <= (size_t)n) new_cap *= 2;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();             /* out‑of‑memory abort */
    cache = tmp;
    cache_cap = new_cap;
}

/* initialize the first 11 terms */
static void init_cache(void)
{
    if (!cache) {
        cache = (seq_t*)malloc(INITIAL_CAP * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 11; ++i) cache[i] = initial[i];
        cache_cap = INITIAL_CAP;
    }
}

/* Core recurrence: computes a(n) for n >= 11 */
static seq_t recurrence(idx_t n)
{
    int64_t val = 0;
    val +=  3LL * (int64_t)cache[n-1];
    val -=  4LL * (int64_t)cache[n-2];
    val +=  3LL * (int64_t)cache[n-3];
    val -=  1LL * (int64_t)cache[n-4];
    val -=  1LL * (int64_t)cache[n-6];
    val +=  3LL * (int64_t)cache[n-7];
    val -=  4LL * (int64_t)cache[n-8];
    val +=  3LL * (int64_t)cache[n-9];
    val -=  1LL * (int64_t)cache[n-10];
    return (seq_t)val;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n <= 10) return initial[n];

    init_cache();

    if (n >= cache_cap) {
        ensure_cache(n);
    }

    /* fill missing terms up to n */
    for (size_t i = 11; i <= (size_t)n; ++i) {
        cache[i] = recurrence((idx_t)i);
    }

    return cache[(size_t)n];
}
