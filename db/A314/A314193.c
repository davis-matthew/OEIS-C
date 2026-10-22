/*
 * A314193  Coordination sequence Gal.4.141.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-7) - a(n-8) for n > 8 initial values:
 *            a(0)=1, a(1)=5, a(2)=11, a(3)=17, a(4)=21, a(5)=27, a(6)=33,
 *            a(7)=38, a(8)=43
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = 8;

/* initial 9 values */
static const seq_t init_values[9] = {
    (seq_t)1, (seq_t)5, (seq_t)11, (seq_t)17, (seq_t)21,
    (seq_t)27, (seq_t)33, (seq_t)38, (seq_t)43
};

/* Ensure cache has space for indices 0 … n */
static void ensure_cache_size(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 9;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init_values[i];
    }
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        /* zero‑initialize the newly allocated tail */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 9)
        return init_values[(size_t)n];

    ensure_cache_size(n);

    if (n > last_computed) {
        for (idx_t k = last_computed + 1; k <= n; ++k) {
            cache[(size_t)k] = cache[(size_t)(k - 1)] +
                               cache[(size_t)(k - 7)] -
                               cache[(size_t)(k - 8)];
        }
        last_computed = n;
    }
    return cache[(size_t)n];
}
