/*
 * A315628  Coordination sequence Gal.4.104.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-6) + a(n-10) - a(n-16) (for n>21) Initial values:
 *            a(0)=1, a(1)=6, a(2)=12, a(3)=16, a(4)=18, a(5)=28, a(6)=34,
 *            a(7)=34, a(8)=44, a(9)=48, a(10)=54, a(11)=62, a(12)=64,
 *            a(13)=68, a(14)=76, a(15)=82, a(16)=88, a(17)=90, a(18)=98,
 *            a(19)=100, a(20)=110, a(21)=114
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Persistent cache for the computed values */
static seq_t *cache = NULL;          /* array of values */
static size_t cache_capacity = 0;    /* allocated size of the array */
static idx_t  cache_max_computed = 0;/* highest index with a computed value */

/* Initial known values up to a(21) */
static const seq_t init_vals[22] = {
    1, 6, 12, 16, 18, 28, 34, 34,
    44, 48, 54, 62, 64, 68, 76, 82,
    88, 90, 98, 100, 110, 114
};

/* Ensure the cache has room for indices 0..n inclusive */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_capacity) return;

    size_t new_capacity = ((size_t)n + 1) + 16; /* modest overshoot */
    seq_t *new_cache = (seq_t *)malloc(new_capacity * sizeof(seq_t));
    if (!new_cache) { /* allocation failed; abort */
        abort();
    }
    if (cache) {
        for (size_t i = 0; i <= cache_max_computed; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
    }
    cache = new_cache;
    cache_capacity = new_capacity;
}

/* Compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* Lazy initialization of the cache */
    if (!cache) {
        size_t init_capacity = 22 + 16;
        cache = (seq_t *)malloc(init_capacity * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 22; ++i) cache[i] = init_vals[i];
        cache_capacity = init_capacity;
        cache_max_computed = 21;
    }

    if (n <= cache_max_computed) return cache[n];

    /* Allocate more space if needed */
    ensure_capacity(n);

    /* Compute missing terms up to n */
    for (idx_t i = cache_max_computed + 1; i <= n; ++i) {
        if (i <= 21) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = cache[i - 6] + cache[i - 10] - cache[i - 16];
        }
    }
    cache_max_computed = n;
    return cache[n];
}
