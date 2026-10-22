/*
 * A367721  E.g.f. satisfies A(x) = exp(x*A(-x^2)).
 * https://oeis.org/A367721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = (n-1)! * Sum_{k=0..floor((n-1)/2)} (-1)^k *
 *            (2*k+1) * a(k) * a(n-1-2*k) / (k! * (n-1-2*k)!)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"  /* provides ensure_cache(idx_t n, seq_t **p, size_t *cap_ptr) */

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;          /* array of computed a(i) */
    static size_t  capacity  = 0;             /* current allocated size of cache */
    static idx_t   last_computed = (idx_t)-1; /* largest index that has been filled */

    /* Allocate space for indices up to n (inclusive). */
    if (n + 1 > capacity) {
        ensure_cache(n + 1, &cache, &capacity);
    }

    /* Ensure a(0) is available. */
    if (last_computed == (idx_t)-1) {          /* no value has been computed yet */
        cache[0] = 1;
        last_computed = 0;
    }

    /* Compute all missing values up to n. */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        if (m == 0) {                         /* a(0) is already handled */
            cache[m] = 1;
            continue;
        }

        idx_t t        = m - 1;               /* t = n-1 in the formula */
        idx_t max_k    = t / 2;               /* floor((n-1)/2) */
        seq_t sum      = 0;
        seq_t ratio    = 1;                  /* ratio_k = t! / (k! * (t-2k)!) */

        for (idx_t k = 0; k <= max_k; ++k) {
            if (k > 0) {
                /* Update ratio from ratio_{k-1} to ratio_k:
                   ratio_k = ratio_{k-1} * (t-2k+2)*(t-2k+1) / k   */
                seq_t mult1 = (seq_t)(t - 2 * (k - 1));
                seq_t mult2 = (seq_t)(t - 2 * (k - 1) - 1);
                ratio *= mult1;
                ratio *= mult2;
                ratio /= (seq_t)k;            /* exact division */
            }

            /* term = ratio * a(k) * a(t-2k) * (2*k+1) */
            seq_t term = ratio;
            term *= cache[k];
            term *= cache[t - 2 * k];
            term *= (seq_t)(2 * k + 1);

            /* Apply the factor (-1)^k */
            if (k & 1) {
                sum -= term;
            } else {
                sum += term;
            }
        }

        cache[m] = sum;
    }

    last_computed = n;
    return cache[n];
}
