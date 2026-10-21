/*
 * A291399  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - S - S^2 - S^3 -
 *          S^4.
 * https://oeis.org/A291399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3, a(2) = 8, a(3) = 22, a(4) = 59, a(5) = 156,
 *            a(6) = 412, a(7) = 1093, a(8) = 2903; for n >= 9: a(n) = a(n-1)
 *            + 2*a(n-2) + 3*a(n-3) + 5*a(n-4) + 7*a(n-5) + 7*a(n-6) +
 *            4*a(n-7) + a(n-8)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 8;    /* indices 0..8 already known */

    /* base values */
    static const seq_t base_vals[9] = {
        1, 3, 8, 22, 59, 156, 412, 1093, 2903
    };

    /* initialise if first call */
    if (!cache) {
        cap = 9;                                        /* capacity at least 9 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);   /* allocate */
        for (size_t i = 0; i < 9; ++i) cache[i] = base_vals[i];
    }

    /* expand capacity if needed */
    size_t required = (size_t)n + 1;                     /* need index n */
    if (required > cap) {
        size_t newcap = cap ? (cap << 1) : 1;
        while (newcap < required) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t)*newcap);
        cache = tmp;
        cap = newcap;
    }

    /* compute missing values up to n */
    if (n > (idx_t)max_computed) {
        for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
            seq_t val = cache[i-1];
            val += 2 * cache[i-2];
            val += 3 * cache[i-3];
            val += 5 * cache[i-4];
            val += 7 * cache[i-5];
            val += 7 * cache[i-6];
            val += 4 * cache[i-7];
            val += cache[i-8];
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
