/*
 * A295383  a(n) = (2*n)! * [x^(2*n)] (-x/(1 - x))^n/((1 - x)*n!).
 * https://oeis.org/A295383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * binomial(2*n,n)^2 * n!
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   binomial, factorial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Compute binomial(2*n, n) */
    seq_t bin = binomial(2 * n, n);
    /* Square it: bin^2 */
    seq_t bin_sq = seq_mul(bin, bin);
    /* Factorial of n */
    seq_t fact = factorial(n);
    /* Multiply all parts: n! * bin^2 */
    seq_t res = seq_mul(bin_sq, fact);
    /* Apply the sign (-1)^n */
    if (n & 1) res = -res;
    return res;
}
