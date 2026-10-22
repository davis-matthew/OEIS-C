/*
 * A350034  a(n) = n/gcd(n,6) if gcd(n,6)>1, 5n + 1 otherwise.
 * https://oeis.org/A350034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/gcd(n,6) if gcd(n,6)>1, 5n + 1 otherwise
 * Range:     u8 n <= 52, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t g  = gcd(sn, 6);
    if (g > 1) {
        return sn / g;
    } else {
        return sn * 5 + 1;
    }
}
