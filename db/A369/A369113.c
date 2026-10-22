/*
 * A369113  Tropical version of Somos-6 sequence A006722.
 * https://oeis.org/A369113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = max( a(n-1) + a(n-5), * a(n-2) + a(n-4), * 2*a(n-3) ) -
 *            a(n-6) * Offset: 0 * initial terms: a(0) = -1, * a(1) = 0, *
 *            a(2) = 0, * a(3) = 0, * a(4) = 0, * a(5) = 0 * * The
 *            implementation memoises already computed values in a *
 *            dynamically sized static array so that any call order is *
 *            suppor...
 * Range:     i8 n <= 71, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;     /* pointer to array of seq_t values   */
    static idx_t cap   = 0;         /* current allocated size             */
    static idx_t last  = 0;         /* largest n that has been computed   */
    static bool  init   = false;    /* whether the initial six terms have been set */

    /* 1. Make sure the cache can hold indices 0..n */
    if (cap <= n) {
        idx_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – should not happen */
        cache = tmp;
        cap   = new_cap;
    }

    /* 2. Initialise first six terms once */
    if (!init) {
        cache[0] = (seq_t)(-1);
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)0;
        cache[5] = (seq_t)0;
        last    = 5;
        init    = true;
    }

    /* 3. Compute any missing values up to n (if any) */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t m1 = cache[i-1] + cache[i-5];
        seq_t m2 = cache[i-2] + cache[i-4];
        seq_t m3 = cache[i-3] * (seq_t)2;

        seq_t m = m1 > m2 ? m1 : m2;
        if (m3 > m) m = m3;

        cache[i] = m - cache[i-6];
    }

    /* 4. Record the largest computed index */
    if (n > last) last = n;

    return cache[n];
}
