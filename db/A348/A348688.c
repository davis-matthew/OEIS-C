/*
 * A348688  a(n) = sigma(n) + sigma(n+1) + sigma(n+2) - sigma(n+3), where
 *          sigma is the sum of divisors.
 * https://oeis.org/A348688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(n) + sigma(n+1) + sigma(n+2) - sigma(n+3)
 * Range:     i8 n <= 33, i16 n <= 6117, i32 n <= 9949, i64 n <= 9949,
 *            i128 n <= 9949, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

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
