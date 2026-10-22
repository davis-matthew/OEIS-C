/*
 * A350513  Number of 2 X n rectangular Celtic knots (up to rotations and
 *          symmetries).
 * https://oeis.org/A350513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(3*n-2) + 3^floor((3*n-1)/2) + 3^(2*n-1) +
 *            3^floor(3*n/2)) / 4 for n > 2 Special cases: a(1)=3, a(2)=21
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Handle the two known special terms */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)21;

    /* Constants */
    const seq_t three = (seq_t)3;

    /* Exponents (using idx_t arithmetic, then cast to seq_t) */
    seq_t exp1 = (seq_t)(3 * n - 2);           /* 3*n-2                   */
    seq_t exp2 = (seq_t)((3 * n - 1) / 2);     /* floor((3*n-1)/2)        */
    seq_t exp3 = (seq_t)(2 * n - 1);           /* 2*n-1                   */
    seq_t exp4 = (seq_t)((3 * n) / 2);         /* floor(3*n/2) (i.e. 3*n/2 integer div) */

    /* Compute the powers of 3 */
    seq_t p1 = big_pow(three, exp1);
    seq_t p2 = big_pow(three, exp2);
    seq_t p3 = big_pow(three, exp3);
    seq_t p4 = big_pow(three, exp4);

    /* Sum and divide by 4 (the numerator is always divisible by 4) */
    seq_t sum = p1 + p2 + p3 + p4;
    return sum / (seq_t)4;
}
