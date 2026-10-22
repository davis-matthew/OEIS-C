/*
 * A315213  Coordination sequence Gal.6.453.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-5) - a(n-10), a(0)=1, a(1)=6, a(2)=10, a(3)=14,
 *            a(4)=20, a(5)=26, a(6)=32, a(7)=34, a(8)=38, a(9)=46, a(10)=52
 *            (for n > 10)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t cache_max = 0;

static const seq_t base_vals[11] = { (seq_t)1, (seq_t)6, (seq_t)10, (seq_t)14,
                                     (seq_t)20, (seq_t)26, (seq_t)32, (seq_t)34,
                                     (seq_t)38, (seq_t)46, (seq_t)52 };

static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_capacity) return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp;
#if defined(__cplusplus)
    tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#else
    tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!tmp) abort();
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* Lazy initialization of cache with base values */
    if (!cache) {
        cache_capacity = 11;
#if defined(__cplusplus)
        cache = (seq_t*)malloc(cache_capacity * sizeof(seq_t));
#else
        cache = malloc(cache_capacity * sizeof(seq_t));
#endif
        if (!cache) abort();
        for (size_t i = 0; i < 11; ++i) cache[i] = base_vals[i];
        cache_max = 10;
    }

    if ((size_t)n <= (size_t)cache_max) {
        return cache[(size_t)n];
    }

    size_t needed = (size_t)(n + 1);
    ensure_cache_size(needed);

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[(size_t)i] = base_vals[(size_t)i];
        } else {
            cache[(size_t)i] = (seq_t)2 * cache[(size_t)(i - 5)] - cache[(size_t)(i - 10)];
        }
    }
    cache_max = n;
    return cache[(size_t)n];
}
