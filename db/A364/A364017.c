/*
 * A364017  Expansion of Sum_{k>0} (-1)^(k-1) * x^(3*k-2) / (1 - x^(3*k-2))^3.
 * https://oeis.org/A364017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -Sum_{d|n, (n/d)≡1 (mod 3)} (-1)^(n/d) * binomial(d+1,2)
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n=0, return 0 as safe default */
        return (seq_t)0;

    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* first divisor d */
        idx_t m1 = n / d;
        if (m1 % 3 == 1) {
            seq_t binom1 = ((seq_t)(d + 1) * (seq_t)d) / 2;
            result += (m1 & 1) ? binom1 : -binom1;
        }

        /* paired divisor n/d, avoid double‑counting the square root */
        if (d != m1) {
            idx_t d2 = m1;   /* divisor n/d */
            idx_t m2 = d;    /* n/(n/d) */
            if (m2 % 3 == 1) {
                seq_t binom2 = ((seq_t)(d2 + 1) * (seq_t)d2) / 2;
                result += (m2 & 1) ? binom2 : -binom2;
            }
        }
    }
    return result;
}
