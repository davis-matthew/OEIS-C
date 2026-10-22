/*
 * A360162  a(n) is the sum of the square roots of the unitary divisors of n
 *          that are squares.
 * https://oeis.org/A360162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n, gcd(d,n/d)=1, d square} sqrt(d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = product_{p^e||n} (1 + p^{e/2}) if e even, 1 otherwise. */
/* a(n) is multiplicative; this implementation uses trial division for factorisation. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;               /* product over prime factors */
    uint64_t u = (uint64_t)n;    /* work on unsigned 64‑bit value */

    for (uint64_t p = 2; p * p <= u; ++p) {
        if (u % p == 0) {
            /* count exponent of prime p */
            uint64_t e = 0;
            while (u % p == 0) {
                u /= p;
                ++e;
            }
            if ((e & 1) == 0) {                 /* e is even */
                /* compute p^(e/2) in seq_t */
                seq_t pow = 1;
                for (uint64_t i = 0; i < e / 2; ++i) pow *= (seq_t)p;
                res *= (1 + pow);
            }
        }
    }

    /* Remaining prime factor u (if >1) has exponent 1 (odd), contributes 1 */
    return res;
}
