/*
 * A315645  Coordination sequence Gal.6.633.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12, a(0) = 1, a(1) =
 *            6, a(2) = 12, a(3) = 16, a(4) = 22, a(5) = 26, a(6) = 32, a(7) =
 *            36, a(8) = 42, a(9) = 46, a(10) = 52, a(11) = 58, a(12) = 64
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static const seq_t init[13] = {
        (seq_t)1,  (seq_t)6,  (seq_t)12, (seq_t)16, (seq_t)22, (seq_t)26,
        (seq_t)32, (seq_t)36, (seq_t)42, (seq_t)46, (seq_t)52, (seq_t)58,
        (seq_t)64
    };
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_cached = 0;

    if (n <= 12) {
        return init[(idx_t)n];
    }

    /* Ensure cache capacity up to index n. */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap * 2 : 64;
        while (new_cap < (size_t)(n + 1)) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort();
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Initialize base cases if not yet done. */
    if (max_cached < 12) {
        for (idx_t i = 0; i <= 12; ++i) {
            cache[i] = init[i];
        }
        max_cached = 12;
    }

    /* Compute values up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] + cache[i - 11];
        val -= cache[i - 12];
        cache[i] = val;
    }
    max_cached = n;
    return cache[n];
}
