/*
 * A295301  a(n) = n - phi(sigma(n)), where phi = A000010 and sigma = A000203.
 * https://oeis.org/A295301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - phi(sigma(n))
 * Range:     i8 n <= 59, i16 n <= 8325, i32 n <= 8325, i64 n <= 8325,
 *            i128 n <= 8325, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n) {
    seq_t sigma = sigma_of(n);
    seq_t phi_val = calc_phi(sigma);
    return (seq_t)n - phi_val;
}
