/*
 * A325673  First term of n-th difference sequence of (floor(k*r)), r =
 *          -sqrt(7), k >= 0.
 * https://oeis.org/A325673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 21, i32 n <= 35, i64 n <= 67, i128 n <= 132,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A325673: First term of n-th difference sequence of floor(k*(-sqrt(7))),
 *  r = -sqrt(7), k >= 0.  The returned value is a(n) with offset 1.  */
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Need s[0..n] (n+1 elements) */
    size_t len = (size_t)n + 1;
    seq_t *s = (seq_t *)malloc(len * sizeof(seq_t));
    if (!s) return 0;  /* fallback on allocation failure */

    /* Compute s_k = floor(k*r) with r = -sqrt(7) */
    s[0] = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* k*sqrt(7) = sqrt(7*k^2) */
        seq_t ksq = (seq_t)k * (seq_t)k;
        seq_t sq = floor_sqrt(ksq * (seq_t)7);   /* floor(k*sqrt(7)) */
        s[(size_t)k] = -(sq + (seq_t)1);         /* -ceil(k*sqrt(7)) */
    }

    /* Iteratively compute differences.  After i steps, the first (len-i) entries
     * correspond to Δ^i s[0..].  The required a(n) is Δ^n s[0], i.e. arr[0] after n steps. */
    for (size_t step = 1; step <= n; ++step) {
        for (size_t j = 0; j < len - step; ++j) {
            s[j] = s[j + 1] - s[j];
        }
    }

    seq_t result = s[0];
    free(s);
    return result;
}
