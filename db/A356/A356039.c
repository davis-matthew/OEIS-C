/*
 * A356039  a(n) = Sum_{k=1..n} binomial(n,k) * sigma_3(k).
 * https://oeis.org/A356039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * sigma_3(k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-01)
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
    seq_t sum = 0;
    seq_t bin = 1;                     /* binomial(n,0) */
    seq_t n_seq = (seq_t)n;            /* convert n once */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* binomial(n,k) = binomial(n,k-1) * (n-k+1) / k   */
        seq_t factor = n_seq - (seq_t)k + 1;
        bin = (bin * factor) / (seq_t)k;

        /* sigma_3(k) = sum of d^3 over all divisors d of k */
        seq_t sigma = sigma_k(k, 3);

        sum += bin * sigma;
    }
    return sum;
}
