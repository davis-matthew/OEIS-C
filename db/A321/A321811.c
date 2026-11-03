/*
 * A321811  Sum of 7th powers of odd divisors of n.
 * https://oeis.org/A321811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma_7(odd_part(n)) where odd_part(n) is n with all
 *            factors of 2 removed. sigma_7(m) = Σ_{d|m} d^7, multiplicative:
 *            for m = ∏p_i^e_i, sigma_7(m) = ∏ (p_i^(7*e_i+7)-1)/(p_i^7-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 24, u64 n <= 566,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 45 terms through n = 9549 (2026-11-01)
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
    uint64_t odd = (uint64_t)n;
    while ((odd & 1ULL) == 0ULL)
        odd >>= 1U;
    return sigma_k(odd, 7);
}
