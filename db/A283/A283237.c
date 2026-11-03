/*
 * A283237  a(n) = sigma_2(3*n).
 * https://oeis.org/A283237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_k(3*n, 2) (sum of squares of the divisors of 3*n)
 * Range:     u8 n <= 4, u16 n <= 71, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9379 (2026-11-01)
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

/* Return the n-th term of OEIS sequence A283237. */
seq_t a(idx_t n)
{
    /* Compute m = 3*n.  The multiplication is safe for all practical n,
       since n is an unsigned 64‑bit integer and 3*n fits easily in the type. */
    idx_t m = n * 3;

    /* The sigma_k function from utils returns σ_k(m) = sum_{d|m} d^k.
       Here we need σ_2(m), i.e., the sum of squares of the divisors. */
    return sigma_k(m, 2);
}
