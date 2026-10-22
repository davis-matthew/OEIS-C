/*
 * A315316  Coordination sequence Gal.6.324.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12 initial values:
 *            a(0)=1, a(1)=6, a(2)=10, a(3)=16, a(4)=21, a(5)=25, a(6)=31,
 *            a(7)=35, a(8)=40, a(9)=46, a(10)=50, a(11)=56, a(12)=62
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[13] = {
    (seq_t)1,  (seq_t)6,  (seq_t)10, (seq_t)16, (seq_t)21,
    (seq_t)25, (seq_t)31, (seq_t)35, (seq_t)40, (seq_t)46,
    (seq_t)50, (seq_t)56, (seq_t)62
};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed_max = (idx_t)-1; /* invalid */

    /* allocate and initialise if first call */
    if (cache == NULL) {
        cache_cap = 64; /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory: fallback zero */
        /* copy initial 13 terms */
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = init_vals[i];
        computed_max = 12; /* we have terms up to index 12 */
    }

    /* grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1; /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* out of memory: fallback zero */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i < 13) {
                /* already set from init_vals */
                cache[i] = init_vals[i];
            } else {
                seq_t sum = cache[i - 1] + cache[i - 11];
                cache[i] = sum - cache[i - 12];
            }
        }
        computed_max = n;
    }

    return cache[n];
}
