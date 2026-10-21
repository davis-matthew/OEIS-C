/*
 * A242096  a(n) = (n mod 2) * pi( ceiling(n/2)-1 ), where pi is the prime
 *          counting function (A000720).
 * https://oeis.org/A242096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n mod 2) * pi( ceil(n/2)-1 )
 * Range:     u8 n <= 3238, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* For even n, the value is 0. */
    if ((n & 1) == 0)
        return 0;
    /* For odd n, ceil(n/2)-1 = floor(n/2). */
    return compute_pi(n / 2);
}
