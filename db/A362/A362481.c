/*
 * A362481  E.g.f. satisfies A(x) = exp(x - x^3 * A(x)^3).
 * https://oeis.org/A362481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (-1)^k * (3k+1)^(n-2k-1) / (k!
 *            * (n-3k)!) Computation proceeds by iteratively generating the
 *            combinatorial factor C_k = n!/(k!*(n-3k)!). The recurrence
 *            C_{k+1} = C_k * (n-3k)*(n-3k-1)*(n-3k-2) / (k+1) yields integer
 *            values for all k. The required power is evaluat...
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    idx_t kmax = n / 3;
    seq_t sum = (seq_t)0;
    seq_t C   = (seq_t)1;          /* C_0 = 1 */

    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t exp = n - 2 * k - 1;   /* exponent for (3k+1) */
        seq_t base = (seq_t)(3 * k + 1);
        seq_t powv = seq_pow(base, exp);
        seq_t term = C * powv;

        if (k & 1)
            sum -= term;
        else
            sum += term;

        /* update C for the next k, except after the last term */
        if (k < kmax) {
            seq_t mult1 = (seq_t)(n - 3 * k);
            seq_t mult2 = (seq_t)(mult1 - 1);
            seq_t mult3 = (seq_t)(mult1 - 2);
            seq_t denom = (seq_t)(k + 1);
            C = C * mult1 * mult2 * mult3 / denom;  /* exact division */
        }
    }
    return sum;
}
