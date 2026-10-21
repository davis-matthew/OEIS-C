/*
 * A289260  Coefficients in the expansion of 1/([r]-[2r]x+[3r]x^2-...); [
 *          ]=floor, r=8/5.
 * https://oeis.org/A289260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=5, a(3)=9, a(n) = 2*a(n-1) - a(n-2) +
 *            2*a(n-3) - 2*a(n-4) + p(n) where p(n)=1 for n = 5 or 6 (also the
 *            coefficients of the numerator in the generating function),
 *            otherwise p(n)=0
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 40, u64 n <= 82, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of entries stored */

    /* expand cache if necessary */
    if (n >= cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;                      /* assume realloc succeeds */

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)3;
            } else if (i == 2) {
                cache[i] = (seq_t)5;
            } else if (i == 3) {
                cache[i] = (seq_t)9;
            } else {
                seq_t v = cache[i-1];
                v += v;                    /* 2*a(i-1) */
                v -= cache[i-2];
                seq_t t = cache[i-3];
                t += t;                    /* 2*a(i-3) */
                v += t;
                t = cache[i-4];
                t += t;                    /* 2*a(i-4) */
                v -= t;
                if (i == 5 || i == 6) {
                    v += (seq_t)1;          /* p(i) = 1 for i = 5,6 */
                }
                cache[i] = v;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
