/*
 * A275427  Number of set partitions of [n] such that nine is a multiple of
 *          each block size.
 * https://oeis.org/A275427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=a(n-1)+C(n-1,2)*a(n-3)+C(n-1,8)*a(n-9) where C(x,y)
 *            is the binomial coefficient x choose y. The e.g.f. is exp(x +
 *            x³/6 + x⁹/9!)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 18, u64 n <= 29, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* initialise cache */
    if (!cache) {
        /* start with 64 slots */
        cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) abort();          /* cannot happen in normal operation */
        cache[0] = (seq_t)1;          /* a(0)=1 */
    }

    /* expand cache if necessary */
    if (n + 1 > cap) {
        size_t new_cap = cap;
        while (n + 1 > new_cap) new_cap <<= 1; /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        if (i <= 0) continue;           /* skip, safety for negative indices */
        seq_t val = cache[i-1];          /* a(i-1) term */

        if (i >= 3) {
            /* C(i-1,2) = (i-1)*(i-2)/2 */
            seq_t c = (seq_t)(i-1) * (seq_t)(i-2) / 2;
            val += c * cache[i-3];
        }

        if (i >= 9) {
            /* C(i-1,8) = (i-1)*(i-2)*...*(i-8) / 8!  (8! = 40320) */
            seq_t c = 1;
            for (idx_t k = 0; k < 8; ++k)
                c *= (seq_t)(i-1 - k);
            c /= 40320;          /* divide by 8! */
            val += c * cache[i-9];
        }

        cache[i] = val;
    }

    return cache[n];
}
