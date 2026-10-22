/*
 * A335689  Denominator of b(n), where b(1)=b(2)=1, and for n>2, b(n) =
 *          (1+b(n-2))/b(n-1).
 * https://oeis.org/A335689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 13, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335689: Denominator of b(n) defined by
   b(1)=b(2)=1,
   b(n) = (1 + b(n-2)) / b(n-1)   (n>2).
   The function returns a(n) = denominator of b(n)
   for all n >= 1. */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* a(1) = a(2) = 1 */
    if (n <= 2) return (seq_t)1;

    /* rational representation of b(k) = num_k / den_k (lowest terms) */
    seq_t num1 = (seq_t)1, den1 = (seq_t)1;  /* for k = 2 */
    seq_t num2 = (seq_t)1, den2 = (seq_t)1;  /* for k = 1 */

    for (idx_t k = 3; k <= n; ++k)
    {
        /* num_k = den_{k-1} * (den_{k-2} + num_{k-2}) */
        seq_t temp = den2 + num2;          /* = den_{k-2} + num_{k-2} */
        seq_t num   = den1 * temp;

        /* den_k = num_{k-1} * den_{k-2} */
        seq_t den   = num1 * den2;

        /* reduce to lowest terms */
        seq_t g = seq_gcd(num, den);
        num   /= g;
        den   /= g;

        /* shift for next iteration */
        num2 = num1;  den2 = den1;
        num1 = num;   den1 = den;
    }

    return den1;
}
