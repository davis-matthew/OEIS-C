/*
 * A378994  a(n) = gcd(2*n, sigma(2*n)).
 * https://oeis.org/A378994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(2*n, sigma(2*n))
 * Range:     u8 n <= 53, u16 n <= 9938, u32 n <= 9938, u64 n <= 9938,
 *            u128 n <= 9938, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    idx_t m = n << 1;                 /* 2*n */
    seq_t sigma = sigma_of(m);        /* sigma(2*n) */
    return gcd((seq_t)m, sigma);      /* gcd(2*n, sigma(2*n)) */
}
