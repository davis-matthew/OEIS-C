/*
 * A344753  a(n) = sigma(n) + psi(n) - 2n = Sum_{d|n, d<n} d+(mu(n/d)^2 * d),
 *          where mu is Möbius mu-function.
 * https://oeis.org/A344753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) + psi(n) - 2*n where sigma is the sum of
 *            divisors and psi the Dedekind psi function
 * Range:     u8 n <= 48, u16 n <= 9858, u32 n <= 9858, u64 n <= 9858,
 *            u128 n <= 9858, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(uint64_t base, idx_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= (seq_t)base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* invalid index, return 0 */
    seq_t sigma = 1;                 /* product of sigma factors */
    seq_t psi   = 1;                 /* product of psi factors */
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* sigma factor: (p^(e+1)-1)/(p-1) */
            seq_t p_pow_e_plus1 = pow_seq(p, e + 1);
            seq_t sigma_factor = (p_pow_e_plus1 - 1) / ((seq_t)p - 1);
            sigma *= sigma_factor;

            /* psi factor: p^(e-1)*(p+1) */
            seq_t p_pow_e_minus1 = pow_seq(p, (e > 1) ? e - 1 : 0);
            seq_t psi_factor = p_pow_e_minus1 * ((seq_t)p + 1);
            psi *= psi_factor;
        }
    }

    if (temp > 1) {                   /* remaining prime factor */
        seq_t p = temp;                /* exponent e = 1 */

        /* sigma factor: (p^2-1)/(p-1) = p+1 */
        sigma *= (p + 1);

        /* psi factor: p^(1-1)*(p+1) = p+1 */
        psi *= (p + 1);
    }

    seq_t result = sigma + psi - 2 * (seq_t)n;
    return result;
}
