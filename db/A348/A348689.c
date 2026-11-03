/*
 * A348689  a(n) = sigma(n) + sigma(n+1) - sigma(n+2), where sigma is the sum
 *          of divisors.
 * https://oeis.org/A348689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma_of(n) + sigma_of(n+1) - sigma_of(n+2)
 * Range:     i8 n <= 58, i16 n <= 9238, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9608 (2026-11-01)
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
    /* The OEIS offset is 1; for n < 1 behaviour is irrelevant. */
    if (n == 0) return 0;
    return sigma_of(n) + sigma_of(n + 1) - sigma_of(n + 2);
}
