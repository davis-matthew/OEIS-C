/*
 * A339966  a(n) = (n+1) / gcd(sigma(n),n+1).
 * https://oeis.org/A339966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (n+1) / gcd(sigma(n), n+1)
 * Range:     u8 n <= 254, u16 n <= 9889, u32 n <= 9889, u64 n <= 9889,
 *            u128 n <= 9889, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t n_plus_one = (seq_t)n + 1;
    seq_t sigma_n = sigma_of(n);
    seq_t g = gcd(sigma_n, n_plus_one);
    return n_plus_one / g;
}
