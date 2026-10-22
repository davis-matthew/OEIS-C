/*
 * A308707  a(n) = gcd(n, phi(n) + sigma(n)), where phi is A000010 and sigma
 *          is A000203.
 * https://oeis.org/A308707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, phi(n) + sigma(n))
 * Range:     u8 n <= 89, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* a(0) not defined in OEIS; return 0 to be safe */
    if (n == 0)
        return 0;

    seq_t sn = (seq_t)n;           /* cast n to seq_t */
    seq_t phi_n = phi(n);          /* Euler's totient */
    seq_t sigma_n = sigma_of(n);   /* sum of divisors */
    seq_t sum = phi_n + sigma_n;   /* phi(n) + sigma(n) */

    return gcd(sn, sum);           /* gcd of n and the sum */
}
