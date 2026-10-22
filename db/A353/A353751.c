/*
 * A353751  a(n) = gcd(n, sigma(sigma(n))), where sigma is the sum of divisors
 *          function.
 * https://oeis.org/A353751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, sigma(sigma(n)))
 * Range:     u8 n <= 53, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute σ(n) */
    seq_t sigma_n = sigma_of(n);
    /* Compute σ(σ(n)) – cast is safe for n≤10⁴ (σ(σ(n)) fits in idx_t) */
    seq_t sigma_sigma = sigma_of((idx_t)sigma_n);
    /* Return gcd(n, σ(σ(n))) */
    return gcd((seq_t)n, sigma_sigma);
}
