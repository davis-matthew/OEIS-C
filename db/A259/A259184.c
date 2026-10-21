/*
 * A259184  a(n) = 1 - sigma(n) + sigma(n)^2.
 * https://oeis.org/A259184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 - sigma(n) + sigma(n)^2
 * Range:     u8 n <= 9, u16 n <= 107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t s   = sigma_of(n);
    seq_t res = s * s;
    res -= s;
    res += 1;
    return res;
}
