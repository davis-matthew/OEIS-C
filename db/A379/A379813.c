/*
 * A379813  a(n) = sigma_1(n) * sigma_3(n).
 * https://oeis.org/A379813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma_1(n) * sigma_3(n)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 203, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 52 terms through n = 9284 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* This implementation is fully multiplicative.
   For each prime power p^e, sigma_k(p^e) = 1 + p^k + p^{2k} + … + p^{ek}.  */
static seq_t sigma_k_prime_power(uint64_t p, uint64_t e, uint8_t k)
{
    /* pk = p^k */
    seq_t pk = 1;
    for (uint8_t i = 0; i < k; ++i)
        pk *= (seq_t)p;

    /* sum = 1 + pk + pk^2 + ... + pk^e */
    seq_t sum   = 1;   /* term for i = 0 */
    seq_t term  = 1;
    for (uint64_t i = 1; i <= e; ++i) {
        term *= pk;
        sum  += term;
    }
    return sum;
}

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
