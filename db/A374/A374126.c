/*
 * A374126  a(n) = gcd(sigma(n), sopfr(n)), where sigma is the sum of
 *          divisors, and sopfr is the sum of prime factors with repetition.
 * https://oeis.org/A374126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), sopfr(n))
 * Range:     u8 n <= 1589, u16 n <= 9714, u32 n <= 9714, u64 n <= 9714,
 *            u128 n <= 9714, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    /* Compute sigma(n) */
    seq_t sigma = sigma_of(n);

    /* Compute sopfr(n): sum of prime factors with repetition */
    seq_t sopfr = 0;
    idx_t m = n;
    if (m > 1) {
        /* Handle factor 2 separately */
        if ((m & 1) == 0) {
            idx_t exp = 0;
            while ((m & 1) == 0) {
                m >>= 1;
                exp++;
            }
            sopfr += (seq_t)2 * (seq_t)exp;
        }
        /* Check odd factors */
        for (idx_t p = 3; p * p <= m; p += 2) {
            if (m % p == 0) {
                idx_t exp = 0;
                while (m % p == 0) {
                    m /= p;
                    exp++;
                }
                sopfr += (seq_t)p * (seq_t)exp;
            }
        }
        /* Remaining prime factor > 1 */
        if (m > 1) {
            sopfr += (seq_t)m;
        }
    }

    /* Return gcd(sigma, sopfr) */
    return gcd(sigma, sopfr);
}
