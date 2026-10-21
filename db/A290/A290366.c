/*
 * A290366  Reduced denominators of the Kirchhoff index for the n-halved cube
 *          graph.
 * https://oeis.org/A290366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, more
 * Formula:   a(n) = denominator of the reduced fraction
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   pow2, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                K(n) = 2^{n‑3} * Σ_{t=1}^{n‑1} C(n,t) / (t (n‑t)) */
/*                where C(n,t) is the binomial coefficient and all arithmetic
 *                is performed in the integer type seq_t.  The result is
 *                the reduced denominator of this rational number. */

#include "../utils/seq_gcd.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Sum S = Σ C(n,t) / (t (n‑t))  (t from 1 to n‑1) */
    seq_t sum_num = 0;          /* numerator of S   */
    seq_t sum_den = 1;          /* denominator of S */

    seq_t binom = 1;            /* C(n,0) */

    for (idx_t t = 1; t <= n-1; ++t) {
        /* advance the binomial coefficient C(n,t) */
        binom = binom * (seq_t)(n - t + 1) / (seq_t)t;   /* exact division */

        seq_t term_num = binom;
        seq_t term_den = (seq_t)t * (seq_t)(n - t);

        /* Add term to the running total:  sum_num/sum_den += term_num/term_den */
        seq_t new_num = sum_num * term_den + sum_den * term_num;
        seq_t new_den = sum_den * term_den;

        seq_t g = seq_gcd(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;
    }

    /* Multiply by 2^{n‑3}.  If n<3 the exponent is negative, so we
     * divide the denominator instead of multiplying the numerator. */
    if (n >= 3) {
        sum_num = sum_num * pow2(n - 3);
    } else {
        sum_den = sum_den * pow2(3 - n);
    }

    /* Final reduction: divide numerator and denominator by their GCD */
    seq_t g2 = seq_gcd(sum_num, sum_den);
    sum_den = sum_den / g2;   /* numerator is not needed after this point */

    return sum_den;
}
