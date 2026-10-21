/*
 * A248643  a(n) = phi(2^n) - phi(n^2), with Euler's totient function phi =
 *          A000010.
 * https://oeis.org/A248643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^(n-1) - n*phi(n)
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow_term = pow2(n - 1);
    seq_t phi_term = phi(n);
    return pow_term - (seq_t)n * phi_term;
}
