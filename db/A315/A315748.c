/*
 * A315748  Coordination sequence Gal.5.294.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence: a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10
 * Initial values: a(0)=1, a(1)=6, a(2)=12, a(3)=18, a(4)=23, a(5)=29,
 *                 a(6)=34, a(7)=40, a(8)=46, a(9)=52, a(10)=58
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for indices 0..10 */
    static const seq_t base[11] = {1,6,12,18,23,29,34,40,46,52,58};

    /* Cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* Direct return for indices below 11 */
    if (n < 11) return base[(size_t)n];

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 32;                      /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* copy base values into cache */
        for (idx_t i = 0; i < 11; ++i) cache[i] = base[i];
        cache_len = 11;
    }

    /* Return cached value if already computed */
    if (n < cache_len) return cache[(size_t)n];

    /* Ensure sufficient capacity */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure (unlikely) */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute sequence terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-9] - cache[i-10];
    }
    cache_len = n + 1;
    return cache[(size_t)n];
}
