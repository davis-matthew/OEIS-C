/*
 * A329726  Number of witnesses for Solovay-Strassen primality test of 2*n+1.
 * https://oeis.org/A329726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = delta(2*n+1) * ∏_{p|2*n+1} gcd(n, p-1) where delta(m) = 2
 *            if nu(m-1, 2) = min_{p|m} nu(p-1, 2) delta(m) = 1/2 if ∃p|m with
 *            nu(p, m) odd and nu(p-1, 2) < nu(m-1, 2) delta(m) = 1 otherwise
 *            nu(x, 2) is the exponent of the highest power of 2 dividing x
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exp2, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/exp2.h"

seq_t a(idx_t n)
{
    /* n is the sequence index; the modulus is 2*n+1. */
    uint64_t k = (uint64_t)n;                /* k = n  (used in gcds) */
    uint64_t M = 2 * k + 1;                  /* odd modulus */

    /* Factorize M: store distinct prime factors and their exponents. */
    uint64_t primes[64];
    uint32_t exps[64];
    uint32_t nf = 0;                         /* number of distinct factors */

    uint64_t temp = M;
    /* Since M is odd, we start at 3 and check only odd divisors. */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint32_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            primes[nf] = p;
            exps[nf]  = e;
            nf++;
        }
    }
    if (temp > 1) {          /* remaining prime factor, if any */
        primes[nf] = temp;
        exps[nf]  = 1;
        nf++;
    }

    /* Product of gcd(k, p-1) over all distinct prime factors p|M. */
    seq_t prod = 1;
    for (uint32_t i = 0; i < nf; ++i) {
        uint64_t g = (uint64_t)gcd((seq_t)k, (seq_t)(primes[i] - 1));
        prod = prod * (seq_t)g;
    }

    /* Exponent of 2 in 2*n = exponent of 2 in M-1. */
    uint32_t expNminus1_2 = 1 + exp2(k);

    /* Determine min nu(p-1, 2) and whether the odd‑exponent condition
       is satisfied. */
    uint32_t minExpPminus1_2 = UINT32_MAX;
    bool oddExponentLower = false;

    for (uint32_t i = 0; i < nf; ++i) {
        uint32_t expPminus1_2 = exp2(primes[i] - 1);
        if (expPminus1_2 < minExpPminus1_2)
            minExpPminus1_2 = expPminus1_2;

        if ((exps[i] & 1U) && expPminus1_2 < expNminus1_2)
            oddExponentLower = true;
    }

    /* Compute delta. */
    seq_t numer = 1;
    seq_t denom = 1;
    if (expNminus1_2 == minExpPminus1_2) {
        numer = 2;
    } else if (oddExponentLower) {
        denom = 2;
    }

    seq_t result = prod * numer;
    if (denom != 1)
        result = result / denom;

    return result;
}
