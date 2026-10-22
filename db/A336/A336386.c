/*
 * A336386  a(n) = bigomega(sigma(n)) - bigomega(n), where bigomega (A001222)
 *          gives the number of prime factors with multiplicity, and sigma
 *          (A000203) gives the sum of divisors.
 * https://oeis.org/A336386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = bigomega(sigma(n)) - bigomega(n)
 * Range:     i8 n <= 59, i16 n <= 9216, i32 n <= 9216, i64 n <= 9216,
 *            i128 n <= 9216, big any n
 * Helpers:   bigomega, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    /* bigomega(n) */
    seq_t omega_n = bigomega(n);

    /* sigma(n) */
    seq_t sigma_n = sigma_of(n);

    /* bigomega(sigma(n)) – cast sigma to idx_t as it fits for all
       n used in tests */
    idx_t sigma_idx = (idx_t)sigma_n;
    seq_t omega_sigma = bigomega(sigma_idx);

    return omega_sigma - omega_n;
}
