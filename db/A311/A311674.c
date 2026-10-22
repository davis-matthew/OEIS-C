/*
 * A311674  Coordination sequence Gal.6.58.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=8, a(3)=13, a(4)=15, a(5)=23, a(6)=25,
 *            a(7)=33, a(8)=37, a(9)=37, a(10)=44, a(11)=48, a(12)=51,
 *            a(13)=56, a(14)=64, a(15)=63, a(16)=72; For n>16:
 *            a(n)=a(n‑1)−a(n‑2)+a(n‑3)−a(n‑4)+a(n‑5)+a(n‑9)−a(n‑10)+a(n‑11)−a(n‑12)+a(n‑13)−a(n‑14)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static cache for memoisation. */
static seq_t *cache = NULL;          /* array of computed values   */
static size_t cache_cap = 0;         /* allocated capacity (in elements)   */
static size_t cache_len = 0;         /* number of values stored (may be 0)  */

/* Initialise the cache with the 17 known values. */
static void init_cache(void)
{
    const seq_t init_vals[17] = {
        1, 4, 8, 13, 15, 23, 25, 33, 37, 37,
        44, 48, 51, 56, 64, 63, 72
    };
    size_t need = 17;
    /* Ensure capacity; use a power‑of‑two size for easy growth. */
    cache_cap = 32;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    for (size_t i = 0; i < need; ++i)
        cache[i] = init_vals[i];
    cache_len = need;
}

/* Grow the cache capacity to at least new_cap. */
static void ensure_cap(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    while (cache_cap < new_cap)
        cache_cap <<= 1;
    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
}

/* Compute the n‑th term, memoising results for later calls. */
seq_t a(idx_t n)
{
    /* First call: initialise cache. */
    if (cache_len == 0)
        init_cache();

    /* Return if already computed. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Extend cache up to n (possible big jump). */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Grow the underlying array if needed. */
        ensure_cap(i + 1);

        seq_t val = cache[(size_t)(i - 1)];
        val -= cache[(size_t)(i - 2)];
        val += cache[(size_t)(i - 3)];
        val -= cache[(size_t)(i - 4)];
        val += cache[(size_t)(i - 5)];
        val += cache[(size_t)(i - 9)];
        val -= cache[(size_t)(i -10)];
        val += cache[(size_t)(i -11)];
        val -= cache[(size_t)(i -12)];
        val += cache[(size_t)(i -13)];
        val -= cache[(size_t)(i -14)];
        cache[(size_t)i] = val;
    }
    cache_len = (size_t)(n + 1);
    return cache[(size_t)n];
}
