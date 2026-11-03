/*
 * A292771  If sigma(n)-phi(n) is even then (sigma(n)-phi(n))/2 otherwise -1.
 * https://oeis.org/A292771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (sigma(n)-phi(n))/2 if sigma(n)-phi(n) even, otherwise -1
 * Range:     i8 n <= 59, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9792 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/totient.h"

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

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t phi_val = totient(n);
    seq_t diff = sigma - phi_val;
    if ((diff & 1) == 0) {
        return diff / 2;
    } else {
        return (seq_t)-1;
    }
}
