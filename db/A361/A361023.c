/*
 * A361023  a(n) = 1 if A007814(sigma(n)) >= A007814(n), otherwise 0, where
 *          A007814(n) gives the 2-adic valuation of n.
 * https://oeis.org/A361023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A007814(sigma(n)) >= A007814(n)] where A007814(k) is the
 *            exponent of 2 in the prime factorisation of k, and sigma(k) is
 *            the sum of the divisors of k. The function returns 1 if the
 *            2‑adic valuation of sigma(n) is at least that of n, otherwise 0
 * Range:     u8 n <= 9357, u16 n <= 9357, u32 n <= 9357, u64 n <= 9357,
 *            u128 n <= 9357, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* Special case: n = 0 is outside the sequence, but we return 0. */
    if (n == 0) return 0;

    /* Compute v2(n). */
    idx_t tmp = n, v2 = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        v2++;
    }

    /* For odd n the condition is always true. */
    if (v2 == 0) return (seq_t)1;

    /* Modulus 2^v2. */
    seq_t mod = ((seq_t)1) << v2;          /* 2^v2 */

    /* Accumulate sigma(n) modulo 2^v2. */
    seq_t sigma_mod = (seq_t)0;
    idx_t d;
    for (d = 1; d <= n / d; ++d) {         /* iterate over divisors up to sqrt(n) */
        if (n % d == 0) {
            sigma_mod += ((seq_t)d) % mod;
            sigma_mod %= mod;

            idx_t other = n / d;
            if (other != d) {
                sigma_mod += ((seq_t)other) % mod;
                sigma_mod %= mod;
            }
        }
    }

    /* sigma_mod indicates whether sigma(n) is divisible by 2^v2. */
    return (sigma_mod == 0) ? (seq_t)1 : (seq_t)0;
}
