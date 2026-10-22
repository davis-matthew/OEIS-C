/*
 * A372815  The square of n minus (the largest divisor d of n with 2 <= d <=
 *          m-1, or 0 if there is no such divisor).
 * https://oeis.org/A372815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 if n is prime; otherwise a(n) = n^2 - n / spf(n) *
 *            where spf(n) is the smallest prime factor of n
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        /* n == 1: no divisor in [2, n-1], so a(1) = 1^2 */
        return (seq_t)1;
    }

    uint64_t spf = _smallest_prime_factor(n);
    seq_t nn = (seq_t)n * (seq_t)n;

    if (spf == n) {
        /* n is prime, largest proper divisor >1 does not exist */
        return nn;
    }

    /* n is composite: largest proper divisor >1 is n / spf */
    seq_t lpd = (seq_t)n / (seq_t)spf;
    return nn - lpd;
}
