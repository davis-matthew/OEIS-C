/*
 * A295303  a(n) = +1 if n > phi(sigma(n)), -1 if n < phi(sigma(n)), and 0 if
 *          n = phi(sigma(n)), where phi = A000010 and sigma = A000203.
 * https://oeis.org/A295303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sign(n - phi(sigma(n)))
 * Range:     i8 n <= 71, i16 n <= 9875, i32 n <= 9875, i64 n <= 9875,
 *            i128 n <= 9875, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* n is >= 1 as per sequence definition */
    seq_t sigma = sigma_of(n);           /* sigma(n) */
    seq_t phi_sigma = calc_phi(sigma);   /* phi(sigma(n)) */
    seq_t N = (seq_t)n;                  /* cast index to seq_t for comparison */

    if (N > phi_sigma)
        return (seq_t)1;
    else if (N < phi_sigma)
        return (seq_t)-1;
    else
        return (seq_t)0;
}
