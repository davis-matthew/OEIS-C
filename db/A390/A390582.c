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
 * Validated: standard, pass, 88 terms through n = 9328 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

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
