/*
 * A361147  a(n) = sigma(n)^3.
 * https://oeis.org/A361147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma(n)^3
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 539, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* here sigma(n) is the sum of all positive divisors of n */

#include <stdint.h>
#include "../utils/seq_pow.h"

/* compute sigma(n) using prime factorisation */
static seq_t sigma_of_n(idx_t n)
{
    seq_t sigma = 1;
    idx_t m = n;

    for (idx_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            idx_t exp = 0;
            while (m % d == 0) {
                m /= d;
                ++exp;
            }
            /* part = (d^(exp+1)-1) / (d-1)  (on seq_t arithmetic) */
            seq_t part_num = seq_pow((seq_t)d, exp + 1) - 1;
            seq_t part_den = (seq_t)d - 1;
            seq_t part = part_num / part_den;
            sigma *= part;
        }
    }

    /* remaining prime factor > 1 */
    if (m > 1) {
        seq_t p = (seq_t)m;            /* m is prime here */
        seq_t part = (seq_pow(p, 2) - 1) / (p - 1);
        sigma *= part;
    }

    return sigma;
}

seq_t a(idx_t n)
{
    seq_t s = sigma_of_n(n);
    seq_t t = s * s;   /* s^2 */
    return t * s;      /* s^3 */
}
