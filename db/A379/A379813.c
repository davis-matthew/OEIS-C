/*
 * A379813  a(n) = sigma_1(n) * sigma_3(n).
 * https://oeis.org/A379813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma_1(n) * sigma_3(n)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 203, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_k_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_k_prime_power.h"

/* Main sequence function requested by the build system */
seq_t a(idx_t n)
{
    if (n == 0)         /* offset is 1, guard for 0 if called */
        return 0;

    seq_t sigma1 = 1;
    seq_t sigma3 = 1;

    uint64_t m = (uint64_t)n;          /* work on a 64‑bit copy */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t s1 = sigma_k_prime_power(p, exp, 1);
            seq_t s3 = sigma_k_prime_power(p, exp, 3);
            sigma1 *= s1;
            sigma3 *= s3;
        }
    }

    /* if there is a remaining prime factor > sqrt(original n) */
    if (m > 1) {
        seq_t s1 = sigma_k_prime_power(m, 1, 1);
        seq_t s3 = sigma_k_prime_power(m, 1, 3);
        sigma1 *= s1;
        sigma3 *= s3;
    }

    return sigma1 * sigma3;
}
