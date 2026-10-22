/*
 * A308397  Expansion of e.g.f. exp(Sum_{k>=1} x^(k^2)*(1 - x^(k^2))/k^2).
 * https://oeis.org/A308397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = Σ_{k^2 ≤ n} (n−1)!/(n−k^2)! *
 *            a(n−k^2) − 2 * Σ_{2k^2 ≤ n} (n−1)!/(n−2k^2)! * a(n−2k^2)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *pref = NULL;        /* factorials: pref[i] = i!          */
    static seq_t *dp   = NULL;        /* sequence values                 */
    static size_t   cur_max = 0;      /* largest n computed so far       */
    if (n == (idx_t)0)
        return (seq_t)1;              /* a(0) = 1                        */

    /* initialise tables on first use */
    if (pref == NULL) {
        pref = (seq_t *)malloc(sizeof(seq_t));
        dp   = (seq_t *)malloc(sizeof(seq_t));
        if (pref == NULL || dp == NULL)
            return (seq_t)0;                 /* out of memory, return 0 */
        pref[0] = (seq_t)1;                  /* 0! = 1                       */
        dp[0]   = (seq_t)1;                  /* a(0) = 1                     */
        cur_max = 0;
    }

    /* enlarge tables if necessary and compute new values */
    if ((size_t)n > cur_max) {
        size_t new_max = (size_t)n;
        pref = (seq_t *)realloc(pref, (new_max + 1) * sizeof(seq_t));
        dp   = (seq_t *)realloc(dp,   (new_max + 1) * sizeof(seq_t));
        if (pref == NULL || dp == NULL)
            return (seq_t)0;                     /* out of memory              */

        /* compute factorials up to n */
        for (idx_t i = cur_max + 1; i <= n; ++i)
            pref[i] = pref[i - 1] * (seq_t)i;

        /* compute a(k) for k = cur_max+1 .. n */
        for (idx_t cur = cur_max + 1; cur <= n; ++cur) {
            seq_t sum = 0;

            /* contribution from k^2 terms */
            for (idx_t k = 1; (k * k) <= cur; ++k) {
                idx_t s = k * k;
                seq_t ratio = pref[cur - 1];
                if (s != cur)          /* avoid division by pref[0] when s==cur */
                    ratio /= pref[cur - s];
                sum += ratio * dp[cur - s];
            }

            /* contribution from 2k^2 terms with a factor -2 */
            for (idx_t k = 1; (2 * k * k) <= cur; ++k) {
                idx_t d = 2 * k * k;
                seq_t ratio = pref[cur - 1] / pref[cur - d];
                seq_t term  = ratio * dp[cur - d];
                term = -2 * term;
                sum += term;
            }

            dp[cur] = sum;
        }
        cur_max = new_max;
    }

    return dp[n];
}
