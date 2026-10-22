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
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n < 1 behaviour is irrelevant. */
    if (n == 0) return 0;
    return sigma_of(n) + sigma_of(n + 1) - sigma_of(n + 2);
}
