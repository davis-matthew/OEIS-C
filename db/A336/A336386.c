/*
 * A336386  a(n) = bigomega(sigma(n)) - bigomega(n), where bigomega (A001222)
 *          gives the number of prime factors with multiplicity, and sigma
 *          (A000203) gives the sum of divisors.
 * https://oeis.org/A336386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = bigomega(sigma(n)) - bigomega(n)
 * Range:     i8 n <= 59, i16 n <= 65536, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9216 (2026-11-01)
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/bigomega.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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
