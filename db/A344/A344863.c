/*
 * A344863  a(n) = mu(sigma(n)).
 * https://oeis.org/A344863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = mobius(sigma(n))
 * Range:     i8 n <= 71, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    seq_t sum = sigma_of(n);          /* sigma(n) */
    seq_t mu  = mobius((idx_t)sum);   /* mobius of sigma(n) */
    return mu;
}
