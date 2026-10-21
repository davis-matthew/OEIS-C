/*
 * A261768  a(n) = phi(n)^n - n^phi(n), where phi(n) is Euler's totient
 *          function.
 * https://oeis.org/A261768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = phi(n)^n - n^phi(n)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 28,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for offsets starting at 1 */
    seq_t phi_n = phi(n);
    seq_t left = big_pow(phi_n, (seq_t)n);  /* phi(n)^n */
    seq_t right = big_pow((seq_t)n, phi_n); /* n^phi(n)    */
    return left - right;
}
