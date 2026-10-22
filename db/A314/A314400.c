/*
 * A314400  Coordination sequence Gal.6.392.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = - a(n-2) + a(n-3) +
 *            a(n-5) - a(n-6) + a(n-7) - a(n-8) + 2*a(n-9) - a(n-10) + a(n-11)
 *            - a(n-12) + a(n-13) + a(n-15) - a(n-16) - a(n-18) for n > 18
 *            initial values: a(0)=1, a(1)=5, a(2)=8, a(3)=11, a(4)=20,
 *            a(5)=23, a(6)=26, a(7)=31, a(8)=36, a(9)=41, a(10...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    /* initial 19 terms */
    static const seq_t base[19] = {
        1, 5, 8, 11, 20, 23, 26, 31, 36, 41, 42, 49, 56,
        59, 60, 65, 74, 75, 78
    };

    if (cache == NULL) {
        cache_size = 19;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 19; ++i) cache[i] = base[i];
    }

    if (n < cache_size) return cache[n];

    /* grow cache to accommodate n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0; /* allocation failed; caller will handle 0 incorrectly */
    cache = tmp;
    idx_t old_size = cache_size;
    cache_size = new_size;

    for (idx_t i = old_size; i <= n; ++i) {
        if (i < 19) {
            cache[i] = base[i];
            continue;
        }
        /* compute using recurrence with positive and negative parts */
        seq_t pos = cache[i - 3];
        pos += cache[i - 5];
        pos += cache[i - 7];
        pos += cache[i - 9];
        pos += cache[i - 9];
        pos += cache[i - 11];
        pos += cache[i - 13];
        pos += cache[i - 15];

        seq_t neg = cache[i - 2];
        neg += cache[i - 6];
        neg += cache[i - 8];
        neg += cache[i - 10];
        neg += cache[i - 12];
        neg += cache[i - 16];
        neg += cache[i - 18];

        cache[i] = pos - neg;
    }
    return cache[n];
}
