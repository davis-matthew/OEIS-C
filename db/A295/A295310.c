/*
 * A295310  a(n) = gcd(n, A062401(n)), where A062401(n) = phi(sigma(n)).
 * https://oeis.org/A295310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, phi(sigma(n)))
 * Range:     u8 n <= 297, u16 n <= 9644, u32 n <= 9644, u64 n <= 9644,
 *            u128 n <= 9644, big any n
 * Helpers:   gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* sigma(n) */
    seq_t sigma = sigma_of(n);
    /* phi(sigma(n)) */
    seq_t phi_val = phi((idx_t)sigma);
    /* gcd(n, phi(sigma(n))) */
    return gcd((seq_t)n, phi_val);
}
