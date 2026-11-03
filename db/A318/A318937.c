/*
 * A318937  a(n) = 16 times the sum of the cubes of the divisors of 2*n+1.
 * https://oeis.org/A318937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16 * σ₃(2*n + 1) (sum of cubes of divisors of 2*n+1)
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 318, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 50 terms through n = 4996 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t sigma_k(idx_t m, uint8_t k)
{
    seq_t res = (seq_t)1;
    idx_t n = m;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }

            /* compute pk = p^k */
            seq_t pk = (seq_t)1;
            for (idx_t i = 0; i < k; ++i) pk *= (seq_t)p;

            /* sum_{i=0}^{exp} (p^k)^i = 1 + pk + pk^2 + ...   */
            seq_t term = (seq_t)0;
            seq_t pow   = (seq_t)1;
            for (idx_t i = 0; i <= exp; ++i) {
                term += pow;
                pow  *= pk;
            }
            res *= term;
        }
    }

    if (n > 1) {               /* remaining prime factor with exponent 1 */
        /* k is 1 or 3, exp = 1 */
        seq_t pk = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) pk *= (seq_t)n;

        seq_t term = (seq_t)0;
        seq_t pow   = (seq_t)1;
        for (idx_t i = 0; i <= (idx_t)1; ++i) {
            term += pow;
            pow  *= pk;
        }
        res *= term;
    }

    return res;
}

seq_t a(idx_t n)
{
    /* m = 2*n + 1 (always odd) */
    idx_t m = (idx_t)((seq_t)2 * (seq_t)n + 1);

    /* σ₃(m) = sum of d³ over all divisors d of m */
    seq_t sigma = sigma_k(m, (uint8_t)3);

    /* a(n) = 16 * σ₃(m) */
    return sigma * (seq_t)16;
}
