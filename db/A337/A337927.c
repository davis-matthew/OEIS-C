/*
 * A337927  a(n) = n / GCD (n, reverse of n).
 * https://oeis.org/A337927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = n / gcd(n, reverse(n))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic in seq_t */
    seq_t sn = (seq_t)n;

    /* Reverse decimal representation of n */
    seq_t revn = (seq_t)rev(n);

    /* Compute gcd in seq_t */
    seq_t g = gcd(sn, revn);

    /* Return the quotient */
    return sn / g;
}
