/*
 * A318829  a(n) = A063994(n) / A049559(n) = (1/gcd(n-1, phi(n))) *
 *          Product_{primes p dividing n} gcd(p-1, n-1).
 * https://oeis.org/A318829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/gcd(n-1, phi(n))) * product_{p|n} gcd(p-1, n-1)
 * Range:     u8 n <= 2127, u16 n <= 9406, u32 n <= 9406, u64 n <= 9406,
 *            u128 n <= 9406, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }

    seq_t n_minus1 = (seq_t)n - 1;
    seq_t phi_n = phi(n);

    seq_t denom = gcd(n_minus1, phi_n);
    seq_t numer = 1;

    idx_t m = n;
    /* factor n by trial division, keeping only distinct primes */
    for (idx_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) {
            /* d is a prime divisor of n */
            seq_t g = gcd((seq_t)(d - 1), n_minus1);
            numer *= g;

            /* remove all powers of d from m */
            while (m % d == 0) {
                m /= d;
            }
        }
    }
    /* any remaining m > 1 is a prime factor */
    if (m > 1) {
        seq_t g = gcd((seq_t)(m - 1), n_minus1);
        numer *= g;
    }

    return numer / denom;
}
