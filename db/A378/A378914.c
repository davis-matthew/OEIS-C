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
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"

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
