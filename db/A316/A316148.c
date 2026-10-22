/*
 * A316148  Number of non-congruent solutions of x^2+y^2 == z^2+w^2 (mod n).
 * https://oeis.org/A316148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} A_p(e), where * A_2(e) = 2^(2e+1) * (2^e - 1),
 *            * A_p(e) = p^(3e) + p^(3e-1) - p^(2e-1) (p odd prime)
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1199, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* safety: off‑offset values are irrelevant */

    seq_t result = (seq_t)1;
    idx_t remaining = n;

    /* handle factor 2 */
    if ((remaining & 1) == 0) {
        idx_t e = 0;
        while ((remaining & 1) == 0) {
            ++e;
            remaining >>= 1;
        }
        seq_t pow2_2e1 = big_pow((seq_t)2, (seq_t)(2 * e + 1));
        seq_t pow2_e   = big_pow((seq_t)2, (seq_t)e);
        seq_t term     = pow2_2e1 * (pow2_e - (seq_t)1);
        result *= term;
    }

    /* handle odd prime factors */
    for (idx_t p = 3; p * p <= remaining; p += 2) {
        if (remaining % p == 0) {
            idx_t e = 0;
            while (remaining % p == 0) {
                ++e;
                remaining /= p;
            }
            seq_t powp_3e  = big_pow((seq_t)p, (seq_t)(3 * e));
            seq_t powp_3e1 = big_pow((seq_t)p, (seq_t)(3 * e - 1));
            seq_t powp_2e1 = big_pow((seq_t)p, (seq_t)(2 * e - 1));
            seq_t term = powp_3e + powp_3e1 - powp_2e1;
            result *= term;
        }
    }

    /* remaining prime factor (if any) */
    if (remaining > 1) {
        idx_t p = remaining;                 /* e = 1 */
        seq_t powp_3  = big_pow((seq_t)p, (seq_t)3);
        seq_t powp_2  = big_pow((seq_t)p, (seq_t)2);
        seq_t powp_1  = (seq_t)p;            /* p^(2*1-1) = p^1 */
        seq_t term    = powp_3 + powp_2 - powp_1;
        result *= term;
    }

    return result;
}
