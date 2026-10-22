/*
 * A390258  Multiplicative sequence a(n) with a(p^e) = p^(e-1) * (e + 1) * (p
 *          * (2 + e) - e) / 2 for prime p and e > 0.
 * https://oeis.org/A390258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(p^e) = p^(e-1) * (e + 1) * (p * (2 + e) - e) / 2 for prime p,
 *            e > 0
 * Range:     u8 n <= 23, u16 n <= 671, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* trial division */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p != 0)
            continue;
        /* count exponent */
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        /* compute p^(e-1) */
        seq_t pow_p = (seq_t)1;
        for (idx_t i = 0; i < e - 1; ++i)
            pow_p *= (seq_t)p;

        /* compute the prime-power contribution */
        seq_t term = (pow_p *
                      (seq_t)(e + 1) *
                      ((seq_t)p * (seq_t)(e + 2) - (seq_t)e)) /
                     (seq_t)2;

        result *= term;
    }

    /* if m > 1 it is a remaining prime factor with exponent 1 */
    if (m > 1) {
        seq_t p = (seq_t)m;
        seq_t pow_p = (seq_t)1;                     /* e = 1 */
        seq_t term = (pow_p *
                      (seq_t)(1 + 1) *
                      ((seq_t)p * (seq_t)(1 + 2) - (seq_t)1)) /
                     (seq_t)2;
        result *= term;
    }

    return result;
}
