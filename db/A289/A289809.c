/*
 * A289809  p-INVERT of (1,2,1,3,1,4,1,5,...) (A133622), where p(S) = 1 - S -
 *          S^2.
 * https://oeis.org/A289809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 7*a(n-2) + a(n-3) - 9*a(n-4) - 3*a(n-5) +
 *            5*a(n-6) + a(n-7) - a(n-8); initial values: a(0)=1, a(1)=4,
 *            a(2)=12, a(3)=38, a(4)=114, a(5)=354, a(6)=1076, a(7)=3311 The
 *            function memoizes computed values and works for any order of
 *            calls
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial eight terms */
    static const seq_t init[8] = { (seq_t)1, (seq_t)4, (seq_t)12, (seq_t)38,
                                   (seq_t)114, (seq_t)354, (seq_t)1076, (seq_t)3311 };

    /* Handle trivial cases */
    if (n < 8)
        return init[n];

    /* Static storage for memoization */
    static seq_t *cache = NULL;
    static size_t cap    = 0;            /* allocated size */
    static idx_t maxc    = (idx_t)-1;    /* highest index computed */

    /* Ensure sufficient allocation */
    if (!cache || cap < (size_t)(n + 1)) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)(n + 1))
            newcap = n + 1;
        if (newcap < 8)
            newcap = 8;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Initialize cached array if first use */
    if (maxc == (idx_t)-1) {
        for (idx_t i = 0; i < 8 && i < (idx_t)cap; ++i)
            cache[i] = init[i];
        maxc = 7;
    }

    /* Compute missing terms up to n */
    if (n > maxc) {
        for (idx_t i = maxc + 1; i <= n; ++i) {
            seq_t pos = cache[i - 1];
            pos += cache[i - 2] * (seq_t)7;
            pos += cache[i - 3];
            pos += cache[i - 6] * (seq_t)5;
            pos += cache[i - 7];
            seq_t sub = cache[i - 4] * (seq_t)9;
            sub += cache[i - 5] * (seq_t)3;
            sub += cache[i - 8];
            cache[i] = pos - sub;
        }
        maxc = n;
    }

    return cache[n];
}
