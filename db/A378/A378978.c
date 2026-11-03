/*
 * A378978  Deficiency of even numbers: a(n) = 4*n - sigma(2*n).
 * https://oeis.org/A378978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 4*n - sigma(2*n)
 * Range:     i8 n <= 83, i16 n <= 9239, i32 n <= 32769, i64 n <= 32769,
 *            i128 n <= 32769, big any n
 * Validated: standard, pass, 93 terms through n = 9994 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

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
    /* Compute 4*n as a signed integer of type seq_t */
    seq_t four_n = (seq_t)n * 4;
    /* Sum of divisors of 2*n */
    seq_t sum = sigma_of((idx_t)((n << 1)));
    /* Difference gives the deficiency */
    return four_n - sum;
}
