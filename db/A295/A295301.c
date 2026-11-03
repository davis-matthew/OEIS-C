/*
 * A295301  a(n) = n - phi(sigma(n)), where phi = A000010 and sigma = A000203.
 * https://oeis.org/A295301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - phi(sigma(n))
 * Range:     i8 n <= 59, i16 n <= 32399, i32 n <= 32768, i64 n <= 32768,
 *            i128 n <= 32768, big any n
 * Validated: standard, pass, 96 terms through n = 8325 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/totient_seq.h"

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
    seq_t sigma = sigma_of(n);
    seq_t phi_val = totient_seq(sigma);
    return (seq_t)n - phi_val;
}
