/*
 * A339964  a(n) = gcd(sigma(n), n+1).
 * https://oeis.org/A339964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), n+1)
 * Range:     u8 n <= 256, u16 n <= 9770, u32 n <= 9770, u64 n <= 9770,
 *            u128 n <= 9770, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t np1 = ((seq_t)n) + 1;
    return gcd(sigma_of(n), np1);
}
