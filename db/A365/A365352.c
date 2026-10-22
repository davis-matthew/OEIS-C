/*
 * A365352  Number of ways to tile a hexagonal strip made up of n equilateral
 *          triangles, using singles and triples.
 * https://oeis.org/A365352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-4) + a(n-8) + 3*a(n-12) - 3*a(n-16) + 3*a(n-20) -
 *            a(n-24) with initial values a(0)=1, a(1)=1, a(2)=1, a(3)=2,
 *            a(4)=3, a(5)=4, a(6)=10, a(7)=16, a(8)=23, a(9)=29, a(10)=70,
 *            a(11)=113, a(12)=166, a(13)=211, a(14)=505, a(15)=813,
 *            a(16)=1191, a(17)=1515, a(18)=3631, a(19)=5846, a(20)=856...
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Base values for 0 ≤ n < 24. */
static const seq_t base[24] = {
    1, 1, 1, 2, 3, 4, 10, 16,
    23, 29, 70, 113, 166, 211, 505, 813,
    1191, 1515, 3631, 5846, 8566, 10894,
    26105, 42032
};

/* Memoisation cache. */
static seq_t *cache   = NULL;          /* array of cached a(n) values          */
static size_t cache_cap = 0;           /* current capacity (number of slots)   */
static size_t cache_max = (size_t)-1;  /* largest index that has been computed */

/* Ensure the cache can address indices up to 'n' (inclusive). */
static void ensure_cache_cap(idx_t n)
{
    size_t target = (size_t)n;
    if (cache_cap > target)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap <= target)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the base values in the cache once. */
static void init_base_values(void)
{
    for (size_t i = 0; i < 24; ++i)
        cache[i] = base[i];
    cache_max = 23;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n < 24)
        return base[(size_t)n];

    /* Initialise cache on first use. */
    if (cache_max == (size_t)-1)
    {
        ensure_cache_cap(23);          /* ensure at least 24 slots */
        init_base_values();
    }

    ensure_cache_cap(n);

    /* Compute any missing terms up to n. */
    if (cache_max < (size_t)n)
    {
        for (size_t i = cache_max + 1; i <= (size_t)n; ++i)
        {
            seq_t v = cache[i - 4];
            v *= (seq_t)7;
            v += cache[i - 8];
            v += (seq_t)3 * cache[i - 12];
            v -= (seq_t)3 * cache[i - 16];
            v += (seq_t)3 * cache[i - 20];
            v -= cache[i - 24];
            cache[i] = v;
        }
        cache_max = (size_t)n;
    }

    return cache[(size_t)n];
}
