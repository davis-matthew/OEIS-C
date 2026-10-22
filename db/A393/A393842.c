/*
 * A393842  Number of n X n binary matrices with exactly 12 1's such that
 *          there are an even number of 1's in every row and column.
 * https://oeis.org/A393842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 67950*C(n,6)^2 + 70200*C(n,6)*C(n,5) +
 *            10260*C(n,6)*C(n,4) + 360*C(n,6)*C(n,3) + 2*C(n,6)*C(n,2) +
 *            11400*C(n,5)^2 + 1680*C(n,5)*C(n,4) + 36*C(n,4)^2 (compile with
 *            support for binomial(n,k) in utils)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 49, u128 n <= 500,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Compute all required binomial coefficients */
    seq_t c2 = binomial(n, 2);
    seq_t c3 = binomial(n, 3);
    seq_t c4 = binomial(n, 4);
    seq_t c5 = binomial(n, 5);
    seq_t c6 = binomial(n, 6);

    seq_t result = 0;

    result += ((seq_t)67950) * c6 * c6;
    result += ((seq_t)70200) * c6 * c5;
    result += ((seq_t)10260) * c6 * c4;
    result += ((seq_t)360)   * c6 * c3;
    result += ((seq_t)2)     * c6 * c2;
    result += ((seq_t)11400) * c5 * c5;
    result += ((seq_t)1680)  * c5 * c4;
    result += ((seq_t)36)    * c4 * c4;

    return result;
}
