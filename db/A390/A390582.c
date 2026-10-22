/*
 * A390582  a(n) = (2*n - sigma(n))/gcd(n, sigma(n)).
 * https://oeis.org/A390582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n - sigma(n))/gcd(n, sigma(n)) * (handled for any
 *            idx_t >= 1)
 * Range:     i8 n <= 59, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Convert index to seq_t to do arithmetic in the required type */
    seq_t n_seq = (seq_t)n;

    /* Compute sigma(n) by the provided utility */
    seq_t sigma = sigma_of(n);

    /* Compute gcd(n, sigma) by the provided utility */
    seq_t g = gcd(n_seq, sigma);

    /* numerator = 2*n - sigma(n) */
    seq_t numerator = n_seq * ((seq_t)2) - sigma;

    /* Final division */
    return numerator / g;
}
