/*
 * A318201  Expansion of Hypergeometric function F(17/12, 25/12; 4; 1728*x) in
 *          powers of x.
 * https://oeis.org/A318201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*12^n/(n!*(n+3)!)) * ∏_{k=0}^{n-1} (12k+17)(12k+25)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i) {
        seq_t i_seq = (seq_t)i;

        /* numerator: 12 * (12i + 17) * (12i + 25) */
        seq_t t1 = 12 * i_seq + 17;
        seq_t t2 = 12 * i_seq + 25;
        seq_t num = t1 * t2;
        num *= (seq_t)12;

        /* denominator: (i+1)*(i+4) */
        seq_t denom = ((seq_t)(i + 1)) * ((seq_t)(i + 4));

        /* cancel common factors with the current result */
        seq_t g = gcd(result, denom);
        result /= g;
        denom /= g;

        /* cancel remaining factors with the numerator */
        g = gcd(num, denom);
        num /= g;
        denom /= g;   /* now denom should be 1 */

        /* multiply and divide (denom is 1 after reductions) */
        result *= num;
    }
    return result;
}
