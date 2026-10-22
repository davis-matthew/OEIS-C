/*
 * A326147  a(n) = gcd(n-A020639(n), sigma(n)-A020639(n)-n), where A020639
 *          gives the smallest prime factor of n, and sigma is the sum of
 *          divisors of n.
 * https://oeis.org/A326147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n - A020639(n), sigma(n) - A020639(n) - n)
 * Range:     u8 n <= 256, u16 n <= 9821, u32 n <= 9821, u64 n <= 9821,
 *            u128 n <= 9821, big any n
 * Helpers:   gcd, spf_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A020639(n) is the smallest prime factor of n. */
/* sigma(n) is the sum of all positive divisors of n. */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/spf_of.h"

/* Sum of divisors of n using trial division factorisation. */
static uint64_t sigma_of_uint64(uint64_t n) {
    if (n == 0) return 0;
    uint64_t res = 1;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t term = 1;      /* p^0 */
            uint64_t sum  = 1;      /* 1 + p + p^2 + ... */
            while (temp % p == 0) {
                temp /= p;
                term *= p;
                sum += term;
            }
            res *= sum;
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        res *= (1 + temp);
    }
    return res;
}

seq_t a(idx_t n) {
    /* Work with unsigned 64‑bit integers first. */
    uint64_t un = (uint64_t)n;
    uint64_t spf = spf_of(un);                 /* A020639(n) */
    uint64_t sigma = sigma_of_uint64(un);      /* sigma(n) */

    /* First argument of the gcd: n - spf */
    uint64_t lhs = un - spf;                   /* non‑negative */

    /* Second argument: |sigma - (spf + n)| */
    uint64_t sum_spf_n = spf + un;
    uint64_t rhs_abs = (sigma >= sum_spf_n) ? (sigma - sum_spf_n)
                                            : (sum_spf_n - sigma);

    /* Compute the gcd using the provided utility (seq_t argument). */
    return gcd((seq_t)lhs, (seq_t)rhs_abs);
}
