/*
 * A340368  Multiplicative with a(p^e) = (p - 1) * (p + 1)^(e-1).
 * https://oeis.org/A340368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p-1)·(p+1)^{e-1} (multiplicative, OEIS
 *            A340368)
 * Range:     u8 n <= 127, u16 n <= 4095, u32 n <= 9988, u64 n <= 9988,
 *            u128 n <= 9988, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1, a(0) unused */

    seq_t result = 1;
    idx_t tmp = n;
    idx_t p = 2;

    while ((seq_t)p * p <= tmp) {
        if (tmp % p == 0) {
            /* count exponent of p */
            size_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* compute (p-1) * (p+1)^(e-1) */
            seq_t term = (seq_t)p - 1;
            if (e > 1) {
                seq_t power = 1;
                for (size_t i = 0; i < e - 1; ++i)
                    power *= ((seq_t)p + 1);
                term *= power;
            }
            result *= term;
        }
        ++p;
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {
        result *= (seq_t)tmp - 1;
    }

    return result;
}
