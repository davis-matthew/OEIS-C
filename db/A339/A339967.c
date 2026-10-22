/*
 * A339967  a(n) = gcd(sigma(n), n+2).
 * https://oeis.org/A339967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), n+2)
 * Range:     u8 n <= 512, u16 n <= 9757, u32 n <= 9757, u64 n <= 9757,
 *            u128 n <= 9757, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sigma_val = sigma_of(n);
    seq_t n_plus_2   = (seq_t)n + 2;
    return gcd(sigma_val, n_plus_2);
}
