/*
 * A295313  a(n) = gcd(sigma(n), phi(sigma(n))).
 * https://oeis.org/A295313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), phi(sigma(n)))
 * Range:     u8 n <= 328, u16 n <= 9880, u32 n <= 9880, u64 n <= 9880,
 *            u128 n <= 9880, big any n
 * Helpers:   calc_phi, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);
    seq_t p = calc_phi(s);
    return gcd(s, p);
}
