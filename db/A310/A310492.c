/*
 * A310492  Coordination sequence Gal.6.249.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10) for n > 10
 *            Base values: a(0)=1, a(1)=4, a(2)=10, a(3)=16, a(4)=20, a(5)=24,
 *            a(6)=28, a(7)=32, a(8)=38, a(9)=44, a(10)=48
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    /* initial values for n = 0..10 */
    static const seq_t init[11] = {1, 4, 10, 16, 20, 24, 28, 32, 38, 44, 48};

    /* grow the cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (cap == 0 ? 16 : cap);
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – unlikely for tested ranges */
        cache = tmp;
        cap = new_cap;
    }

    /* fill initial values up to 10 if not already */
    if (len < 11) {
        size_t up = (len < 11) ? 11 : len;
        for (size_t i = len; i < up; ++i) cache[i] = init[i];
        len = up;
    }

    /* compute terms up to n using the recurrence */
    for (size_t k = len; k <= (size_t)n; ++k) {
        if (k <= 10) continue;  /* already set */
        cache[k] =  2 * cache[k-1]    - 2 * cache[k-2]
                  + 2 * cache[k-3]    - 2 * cache[k-4]
                  + 2 * cache[k-5]    - 2 * cache[k-6]
                  + 2 * cache[k-7]    - 2 * cache[k-8]
                  + 2 * cache[k-9]    -    cache[k-10];
    }
    len = (size_t)n + 1;
    return cache[(size_t)n];
}
