/*
 * A348945  a(n) = A348944(n) - sigma(n), where A348944 is the arithmetic mean
 *          of A003959 and A034448, and sigma is the sum of divisors function.
 * https://oeis.org/A348945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A003959(n) + A034448(n))/2 - sigma(n), all
 *            multiplicative: A003959(p^e)=(p+1)^e, A034448(p^e)=p^e+1,
 *            sigma(p^e)=1+p+...+p^e
 * Range:     u8 n <= 47, u16 n <= 2047, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 116 terms through n = 9859 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t f = 1, g = 1, s = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;

    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t pw = 1, term = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
                f *= (seq_t)(p + 1);
            }
            g *= (pw + (seq_t)1);
            s *= term;
        }
    }
    if (m > 1) {
        f *= ((seq_t)m + (seq_t)1);
        g *= ((seq_t)m + (seq_t)1);
        s *= ((seq_t)m + (seq_t)1);
    }
    /* (f+g)/2 without overflowing on the sum */
    seq_t mean = f / 2 + g / 2 + (f & g & 1);
    return mean - s;
}
