/*
 * A400350  a(0) = a(1) = 1; a(2*n) = Sum_{j=0..n} a(j) * a(n-j), a(2*n+1) =
 *          a(2*n-1) + a(n).
 * https://oeis.org/A400350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(1) = 1; a(2*n) = Σ_{j=0..n} a(j) * a(n-j); a(2*n+1)
 *            = Σ_{j=0..n} a(j);
 * Range:     u8 n <= 19, u16 n <= 59, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caches for values and prefix sums */
    static seq_t *cache = NULL;
    static seq_t *sum_cache = NULL;
    static size_t cap = 0;

    /* ensure cache size is at least n+1 */
    if (n + 1 > cap) {
        size_t newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure (should not happen) */
        cache = tmp;
        tmp = (seq_t *)realloc(sum_cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        sum_cache = tmp;
        /* compute new elements from old cap to n */
        for (idx_t i = cap; i <= n; ++i) {
            seq_t val;
            if (i == 0 || i == 1) {
                val = (seq_t)1;
            } else if ((i & 1) == 0) {                /* even */
                idx_t half = i >> 1;
                seq_t s = (seq_t)0;
                for (idx_t j = 0; j <= half; ++j) {
                    s += cache[j] * cache[half - j];
                }
                val = s;
            } else {                                 /* odd */
                idx_t half = (i - 1) >> 1;
                val = sum_cache[half];
            }
            cache[i] = val;
            sum_cache[i] = (i == 0) ? val : sum_cache[i - 1] + val;
        }
        cap = newcap;
    }
    return cache[n];
}
