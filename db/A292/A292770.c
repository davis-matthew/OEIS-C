/*
 * A292770  If sigma(n)+phi(n) is even then a(n) = (sigma(n)+phi(n))/2
 *          otherwise -1.
 * https://oeis.org/A292770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (sigma(n)+phi(n))/2 if sigma(n)+phi(n) is even, otherwise
 *            -1 (offset 1)
 * Range:     i8 n <= 47, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9573 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    /* values below the offset are undefined, return -1 to be safe */
    if (n < (idx_t)1)
        return (seq_t)-1;

    /* compute sigma(n) and phi(n) using the utility functions */
    seq_t sigma = sigma_of(n);
    seq_t phi_val = totient(n);

    /* sum and check parity */
    seq_t sum = sigma + phi_val;
    if ((sum % (seq_t)2) == (seq_t)0)
        return sum / (seq_t)2;
    else
        return (seq_t)-1;
}
