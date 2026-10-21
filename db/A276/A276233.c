/*
 * A276233  a(n) = (n+256)/gcd(n,256).
 * https://oeis.org/A276233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+256)/gcd(n,256)
 * Range:     u8 none, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Convert index to sequence type to avoid overflow */
    seq_t sn = (seq_t)n;
    seq_t g = gcd(sn, (seq_t)256);
    seq_t num = sn + (seq_t)256;
    return num / g;
}
