/*
 * A380492  a(n) = n! * Sum_{k=0..n} binomial(2*n-2,k)/(n-k)!.
 * https://oeis.org/A380492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} binomial(2*n-2,k)/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* a(0) = 1 by definition */
    if (n == 0) return (seq_t)1;

    seq_t result  = 0;
    seq_t binom   = 1;  /* binomial(2*n-2, 0) */
    seq_t falling = 1;  /* n!/(n-0)! = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = falling * binom */
        seq_t term = falling * binom;
        result += term;

        if (k == n) break;   /* last iteration finished */

        /* update falling for next k: multiply by (n-k) */
        falling *= (seq_t)(n - k);

        /* update binom for next k: binom * (2*n-2-k) / (k+1) */
        binom = binom * (seq_t)(2 * n - 2 - k) / (seq_t)(k + 1);
    }
    return result;
}
