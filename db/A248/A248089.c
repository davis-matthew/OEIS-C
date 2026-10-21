/*
 * A248089  a(n) = Sum_{k=0..floor(n/4)} binomial(n-k, 3k)*(-3)^(3k)*4^(n-4k).
 * https://oeis.org/A248089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(n-k,
 *            3k)*(-3)^(3k)*4^(n-4k)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 13, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t max_k = n / 4;
    seq_t sum = 0;

    /* Precompute 4^n as the starting value t0 (k = 0). */
    seq_t pow4 = pow_seq(4, (int)n);   /* 4^n  */
    seq_t powMinus27 = 1;              /* (-27)^k  */

    /* k = 0 term: binomial = 1, powMinus27 = 1, pow4 = 4^n */
    sum += pow4;

    for (idx_t k = 1; k <= max_k; ++k) {
        /* Update powers for next k */
        powMinus27 = powMinus27 * (seq_t)(-27);    /* (-27)^k  */
        pow4 = pow4 / (seq_t)256;                 /* 4^(n-4k) */

        /* compute binomial(n-k, 3k) */
        seq_t bin = binomial(n - k, 3 * k);

        /* term = bin * (-27)^k * 4^(n-4k) */
        seq_t term = bin * powMinus27;
        term = term * pow4;

        sum += term;
    }

    return sum;
}
