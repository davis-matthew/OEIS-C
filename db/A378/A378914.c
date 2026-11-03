/*
 * A378914  Smallest positive m such that sigma(m) does not divide n, where
 *          sigma is the sum-of-divisors function (A000203).
 * https://oeis.org/A378914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest positive m such that sigma(m) does not divide n
 * Range:     u8 n <= 1127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9410 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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
    /* The sum-of-divisors function sigma is provided by sigma_of.  We
       simply test successive m until we find one whose sigma does not
       divide the given n.  The result is returned as a seq_t. */
    seq_t sn = (seq_t)n;          /* convert n to seq_t for modulus */
    for (idx_t m = 1; ; ++m) {
        seq_t s = sigma_of(m);
        if (sn % s != 0) {
            return (seq_t)m;
        }
    }
}
