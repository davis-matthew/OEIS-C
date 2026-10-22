/*
 * A313912  Coordination sequence Gal.6.207.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - 2*a(n-10) +
 *            2*a(n-11) - a(n-12), a(0)=1, a(1)=5, a(2)=10, a(3)=16, a(4)=22,
 *            a(5)=27, a(6)=31, a(7)=35, a(8)=40, a(9)=46, a(10)=52, a(11)=57,
 *            a(12)=62
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[13] = {
    1, 5, 10, 16, 22, 27, 31, 35, 40, 46, 52, 57, 62
};

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

static void ensure_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 13;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;        /* idx_t is uint64_t, cast is safe for array index */

    /* initialise cache with the known first 13 terms */
    if (cache_size == 0) {
        ensure_cache(13);
        for (size_t i = 0; i < 13; ++i) cache[i] = init_vals[i];
        cache_size = 13;
    }

    if (idx < cache_size) return cache[idx];

    /* compute terms up to idx, extending the cache */
    size_t new_size = idx + 1;
    ensure_cache(new_size);

    for (size_t i = cache_size; i <= idx; ++i) {
        /* recurrence holds for i >= 13 */
        seq_t sum_pos = cache[i-1] + cache[i-3] + cache[i-5]
                        + cache[i-7] + cache[i-9] + cache[i-11];
        seq_t sum_neg = cache[i-2] + cache[i-4] + cache[i-6]
                        + cache[i-8] + cache[i-10];
        seq_t s = sum_pos - sum_neg;          /* s >= 0 by the recurrence */
        cache[i] = (s << 1) - cache[i-12];   /* 2*s – a(i-12) */
    }

    cache_size = new_size;
    return cache[idx];
}
