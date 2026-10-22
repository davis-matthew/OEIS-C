/*
 * A372931  a(n) = Sum_{1 <= x_1, x_2, x_3, x_4 <= n} gcd(x_1, x_2, x_3, x_4,
 *          n)^4.
 * https://oeis.org/A372931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 139, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = Π_{p^e‖n} [ (e*(p^4-1) + p^4) * p^(4*(e-1)) ]
 * (multiplicative evaluation using the prime‑power formula from OEIS A372931)
 */
#include <stdint.h>

/* exponentiation by squaring – works for any integer type seq_t */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }

            seq_t p_seq = (seq_t)p;
            seq_t p4   = pow_seq(p_seq, 4);
            seq_t term = (seq_t)e * (p4 - 1) + p4;          /* e*(p^4-1)+p^4   */
            term *= pow_seq(p_seq, 4 * (e - 1));            /* *p^(4(e-1))      */
            result *= term;
        }
    }

    if (tmp > 1) { /* remaining prime factor */
        idx_t p = tmp;
        seq_t p_seq = (seq_t)p;
        seq_t p4   = pow_seq(p_seq, 4);
        seq_t term = (1 * (p4 - 1) + p4);                /* e=1  -> 2p^4-1  */
        result *= term;
    }

    return result;
}
