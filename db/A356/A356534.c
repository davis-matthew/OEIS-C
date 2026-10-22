/*
 * A356534  a(n) = sigma_3(n)^2.
 * https://oeis.org/A356534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma_3(n)^2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 39, u64 n <= 1547,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* compute sigma_3(n) */
    seq_t sigma = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            seq_t base = (seq_t)p;
            seq_t num = seq_pow(base, 3 * (e + 1)) - 1;
            seq_t den = seq_pow(base, 3) - 1;
            sigma *= num / den;
        }
    }
    if (m > 1) {            /* remaining prime factor with exponent 1 */
        seq_t base = (seq_t)m;
        seq_t num = seq_pow(base, 6) - 1;   /* 3*(1+1) = 6 */
        seq_t den = seq_pow(base, 3) - 1;
        sigma *= num / den;
    }
    return sigma * sigma;
}
