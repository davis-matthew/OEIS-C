/*
 * A348688  a(n) = sigma(n) + sigma(n+1) + sigma(n+2) - sigma(n+3), where
 *          sigma is the sum of divisors.
 * https://oeis.org/A348688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(n) + sigma(n+1) + sigma(n+2) - sigma(n+3)
 * Range:     i8 n <= 33, i16 n <= 6117, i32 n <= 11000, i64 n <= 11000,
 *            i128 n <= 11000, big any n
 * Validated: standard, pass, 81 terms through n = 9949 (2026-11-01)
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
    /* For n < 1 the function is undefined, but we guard against underflow. */
    if (n == 0)
        return 0;

    /* Compute sigma for n, n+1, n+2, n+3. */
    seq_t s0 = sigma_of(n);
    seq_t s1 = sigma_of(n + 1);
    seq_t s2 = sigma_of(n + 2);
    seq_t s3 = sigma_of(n + 3);

    /* Return the required combination. */
    return s0 + s1 + s2 - s3;
}
