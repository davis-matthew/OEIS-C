/*
 * A363607  Expansion of Sum_{k>0} x^(3*k)/(1-x^k)^4.
 * https://oeis.org/A363607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n} binomial(d,3) = (sigma_3(n) - 3*sigma_2(n) +
 *            2*sigma_1(n)) / 6 where sigma_k(n) = sum_{d|n} d^k
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 1569, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* compute sigma_k(n) for small k (k = 1,2,3) using prime factorisation */
static seq_t sigma_k(idx_t n, uint8_t k)
{
    seq_t result = 1;
    idx_t temp = n;
    idx_t p = 2;
    for (; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* count exponent of p */
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            /* compute sum_{i=0..exp} p^{k*i} iteratively */
            seq_t term = 1;          /* i = 0 contribution */
            seq_t pk_pow = 1;        /* p^{k*i} */
            /* p^k */
            seq_t pk = 1;
            for (int i = 0; i < k; ++i) pk *= (seq_t)p;
            for (int i = 1; i <= exp; ++i) {
                pk_pow *= pk;
                term += pk_pow;
            }
            result *= term;
        }
    }
    if (temp > 1) { /* leftover prime factor with exponent 1 */
        seq_t term = 1 + (seq_t)temp;
        for (int i = 0; i < k; ++i) term *= (seq_t)temp; /* compute p^k */
        seq_t pk = term / (seq_t)temp; /* term is 1 + p^k, recover p^k */
        /* Actually easier: compute 1 + p^k directly */
        term = 1;
        seq_t pk_pow = 1;
        for (int i = 0; i < k; ++i) pk_pow *= temp;
        term += pk_pow;
        result *= term;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* handle n == 0 as undefined; but offset starts at 1 */
    if (n == 0) return 0;

    seq_t sigma1 = sigma_k(n, 1);
    seq_t sigma2 = sigma_k(n, 2);
    seq_t sigma3 = sigma_k(n, 3);

    seq_t num = sigma3 - (seq_t)3 * sigma2 + (seq_t)2 * sigma1;
    return num / 6;
}
