/*
 * A374929  Expansion of Sum_{1<=i<=j} q^(i+j)/( (1-q^i)*(1-q^j) )^2.
 * https://oeis.org/A374929
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7*sigma_3(n) - (6*n+1)*sigma(n))/24
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 51 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A374929. Requires util functions sigma_of() and sigma_k(). */

#include <stdint.h>
#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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
    if (n < 2) return 0;          /* sequence starts at a(2) */

    seq_t sigma1 = sigma_of(n);   /* σ(n) */
    seq_t sigma3 = sigma_k(n, 3); /* σ₃(n) */

    seq_t n_val = (seq_t)n;
    seq_t term1 = 7 * sigma3;
    seq_t term2 = (6 * n_val + 1) * sigma1;
    seq_t numerator = term1 - term2;

    return numerator / (seq_t)24;
}
