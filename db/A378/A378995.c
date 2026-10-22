/*
 * A378995  Numerator of sigma(2*n)/(2*n).
 * https://oeis.org/A378995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of sigma(2*n)/(2*n)
 * Range:     u8 n <= 53, u16 n <= 8976, u32 n <= 8976, u64 n <= 8976,
 *            u128 n <= 8976, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* 2*n can be computed safely as bit shift */
    idx_t m = n << 1;                      /* m = 2*n */
    seq_t sigma = sigma_of(m);             /* sigma(2*n) */
    seq_t denom = (seq_t)m;                /* 2*n as seq_t */
    seq_t g = gcd(sigma, denom);           /* gcd(sigma, 2*n) */
    return sigma / g;                      /* numerator after reduction */
}
