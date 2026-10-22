/*
 * A367719  E.g.f. satisfies A(x) = exp(x*A(x^3)).
 * https://oeis.org/A367719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367719
   a(0) = 1
   a(n) = (n-1)! * Sum_{k=0..floor((n-1)/3)} (3k+1) * a(k) * a(n-1-3k) / (k! * (n-1-3k)!),   n >= 1
   Formula from OEIS entry.
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    if (n == 0) {                 /* a(0) = 1 */
        if (cap == 0) {          /* create table, initialize first entry */
            cap = 1;
            cache = (seq_t *)calloc(cap, sizeof(seq_t));
            if (!cache) return (seq_t)0; /* allocation failed: return 0 */
            cache[0] = (seq_t)1;
        } else if (cache[0] == (seq_t)0) {
            cache[0] = (seq_t)1;
        }
        return cache[0];
    }

    /* make sure table has room for up to n */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        /* zero new part */
        for (size_t i = cap; i < newcap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap = newcap;
    }

    /* a(0) must be defined */
    if (cache[0] == (seq_t)0) cache[0] = (seq_t)1;

    /* compute missing values up to n */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (cache[i] != (seq_t)0) continue;    /* already known */

        idx_t m = (idx_t)i - 1;
        idx_t maxk = m / 3;

        seq_t sum = (seq_t)0;
        seq_t t   = (seq_t)1;   /* t = m!/(k!*(m-3k)!) for k=0 */

        for (idx_t k = 0; k <= maxk; ++k) {
            /* term = t * (3k+1) * a(k) * a(m-3k) */
            seq_t term = t;
            term *= (seq_t)(3 * k + 1);
            term *= cache[(size_t)k];
            term *= cache[(size_t)(m - 3 * k)];
            sum += term;

            if (k < maxk) {
                /* update t for next k:
                   t = t * (m-3k)*(m-3k-1)*(m-3k-2) / (k+1)
                */
                seq_t mult1 = (seq_t)(m - 3 * k);
                seq_t mult2 = (seq_t)(m - 3 * k - 1);
                seq_t mult3 = (seq_t)(m - 3 * k - 2);
                t = t * mult1;
                t = t * mult2;
                t = t * mult3;
                t = t / (seq_t)(k + 1);
            }
        }

        cache[(size_t)i] = sum;
    }

    return cache[(size_t)n];
}
