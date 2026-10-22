/*
 * A328959  a(n) = sigma_0(n) - 2 - (omega(n) - 1) * nu(n), where sigma_0 =
 *          A000005, nu = A001221, omega = A001222.
 * https://oeis.org/A328959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma_0(n) - 2 - (nu(n) - 1) * omega(n) where sigma_0 =
 *            number of divisors, nu = total number of prime factors (with
 *            multiplicity), omega = number of distinct prime factors. All
 *            three functions are computed from the prime factorisation of n.
 *            This implementation uses trial division; it is f...
 * Range:     i8 n <= 9864, i16 n <= 9864, i32 n <= 9864, i64 n <= 9864,
 *            i128 n <= 9864, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For safety, n == 0 returns 0. */
    if (n == 0) return 0;

    uint64_t temp = (uint64_t)n;
    seq_t sigma0 = 1;      /* divisor count */
    seq_t omega   = 0;      /* distinct prime factors */
    seq_t nu      = 0;      /* total prime factors with multiplicity */

    /* factor 2 separately */
    if ((temp & 1) == 0) {
        int exp = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            exp++;
        }
        sigma0 *= (seq_t)(exp + 1);
        omega   += 1;
        nu      += exp;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            sigma0 *= (seq_t)(exp + 1);
            omega   += 1;
            nu      += exp;
        }
    }

    /* if something remains, it is a prime factor > sqrt(original n) */
    if (temp > 1) {
        sigma0 *= 2;      /* exponent 1 → (1+1) */
        omega   += 1;
        nu      += 1;
    }

    /* compute a(n) using the corrected formula */
    return sigma0 - 2 - (nu - 1) * omega;
}
