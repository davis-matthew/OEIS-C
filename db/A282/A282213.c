/*
 * A282213  Coefficients in q-expansion of (E_2^3*E_4 - 3*E_2^2*E_6 +
 *          3*E_2*E_4^2 - E_4*E_6)/3456, where E_2, E_4, E_6 are the
 *          Eisenstein series shown in A006352, A004009, A013973,
 *          respectively.
 * https://oeis.org/A282213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^3 * sigma_3(n) where sigma_3(n) = Σ_{d|n} d^3 (σ_3 is
 *            the sum of cubes of divisors)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 39, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the OEIS sequence A282213 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute σ₃(n) via prime factorisation   */
    seq_t sigma = 1;
    idx_t temp = n;
    idx_t p = 2;
    for (; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            /* σ₃(p^cnt) = (p^(3*cnt+3)-1) / (p^3-1) */
            idx_t num_exp = cnt * 3 + 3;
            seq_t num = 1;
            for (idx_t i = 0; i < num_exp; ++i)
                num *= (seq_t)p;
            num -= 1;
            seq_t den = 1;
            for (idx_t i = 0; i < 3; ++i)
                den *= (seq_t)p;
            den -= 1;
            seq_t contrib = num / den;
            sigma *= contrib;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        /* cnt = 1 */
        seq_t num = 1;
        for (idx_t i = 0; i < 6; ++i) /* 3*1+3 = 6 */
            num *= (seq_t)temp;
        num -= 1;
        seq_t den = 1;
        for (idx_t i = 0; i < 3; ++i)
            den *= (seq_t)temp;
        den -= 1;
        seq_t contrib = num / den;
        sigma *= contrib;
    }

    /* a(n) = n^3 * σ₃(n) */
    seq_t n3 = (seq_t)n * n * n;
    return n3 * sigma;
}
