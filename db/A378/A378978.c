/*
 * A378978  Deficiency of even numbers: a(n) = 4*n - sigma(2*n).
 * https://oeis.org/A378978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 4*n - sigma(2*n)
 * Range:     i8 n <= 83, i16 n <= 9239, i32 n <= 9994, i64 n <= 9994,
 *            i128 n <= 9994, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Compute 4*n as a signed integer of type seq_t */
    seq_t four_n = (seq_t)n * 4;
    /* Sum of divisors of 2*n */
    seq_t sum = sigma_of((idx_t)((n << 1)));
    /* Difference gives the deficiency */
    return four_n - sum;
}
