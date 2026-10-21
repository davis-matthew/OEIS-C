/*
 * A288417  a(n) = Sum_{d|n} A000593(n/d).
 * https://oeis.org/A288417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} A000593(n/d) Equivalent multiplicative formula:
 *            a(2^e) = e + 1, for odd prime p: a(p^e) = sum_{i = 0..e} (i + 1)
 *            * p^(e - i), and a(nm) = a(n) * a(m) for coprime n, m
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the sequence is defined for n ≥ 1 */
    seq_t result = 1;
    idx_t temp = n;

    /* trial division factorisation */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            seq_t term;
            if (p == 2) {
                /* a(2^e) = e + 1 */
                term = (seq_t)(e + 1);
            } else {
                /* a(p^e) = sum_{i=0..e} (i+1) * p^(e-i) */
                seq_t p_pow = 1;
                for (int i = 0; i < e; ++i) {
                    p_pow *= (seq_t)p;          /* now p_pow = p^e */
                }
                term = 0;
                for (int i = 0; i <= e; ++i) {
                    term += (seq_t)(i + 1) * p_pow;
                    p_pow /= (seq_t)p;
                }
            }
            result *= term;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        if (temp == 2) {
            /* exponent 1, p = 2 */
            result *= 2;
        } else {
            /* exponent 1, odd prime p */
            /* a(p) = p + 2  (since e = 1) */
            result *= ((seq_t)temp + 2);
        }
    }

    return result;
}
