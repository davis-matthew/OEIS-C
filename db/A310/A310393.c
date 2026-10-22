/*
 * A310393  Coordination sequence Gal.6.193.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=10, a(3)=14, a(4)=19, a(5)=23, a(6)=28,
 *            a(7)=33, a(8)=37, a(9)=42, a(10)=46, and for n>10: a(n) = a(n-1)
 *            + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) - a(n-10)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init[11] = {
    (seq_t)1, (seq_t)4, (seq_t)10, (seq_t)14, (seq_t)19,
    (seq_t)23, (seq_t)28, (seq_t)33, (seq_t)37, (seq_t)42, (seq_t)46
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_cap(size_t needed) {
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a production environment we would handle the error. */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* External function required by the build. */
seq_t a(idx_t n) {
    if (n < cache_len)
        return cache[n];

    ensure_cache_cap((size_t)n + 1);

    for (idx_t m = cache_len; m <= n; ++m) {
        if (m <= 10) {
            cache[m] = init[m];
        } else {
            cache[m] = cache[m-1] + cache[m-3] - cache[m-4]
                - cache[m-6] + cache[m-7] + cache[m-9] - cache[m-10];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
