/*
 * A351879  a(0) = a(1) = 1; a(n) = -Sum_{k=0..n-2} binomial(n-2,k) * a(k) *
 *          a(n-k-2).
 * https://oeis.org/A351879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(1) = 1; a(n) = -Sum_{k=0..n-2} binomial(n-2,k) *
 *            a(k) * a(n-k-2)
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 18, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  mmax = 0;  /* largest n already computed */

    /* initialise the first two terms */
    if (cap < 2) {
        cache = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
        cap = 2;
        cache[0] = 1;
        cache[1] = 1;
        mmax = 1;
    }

    /* ensure the array is large enough for n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* if the value is already known, return it */
    if (n <= mmax) return cache[n];

    /* compute all terms up to n */
    for (idx_t i = mmax + 1; i <= n; ++i) {
        idx_t m = i - 2;
        __int128 sum = 0;
        __int128 bin = 1;             /* C(m,0) */
        for (idx_t k = 0; k <= m; ++k) {
            seq_t ak   = cache[k];
            seq_t anmk = cache[m - k];
            __int128 prod = bin * (__int128)ak * (__int128)anmk;
            sum += prod;

            if (k < m) {
                /* next binomial coefficient C(m,k+1) = C(m,k)*(m-k)/(k+1) */
                bin = bin * (m - k) / (k + 1);
            }
        }
        cache[i] = (seq_t)(-sum);
    }

    mmax = n;
    return cache[n];
}
