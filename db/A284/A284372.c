/*
 * A284372  a(n) = Sum_{d|n, d = 0, 1, or 11 mod 12} d.
 * https://oeis.org/A284372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d ≡ 0,1,11 (mod 12)} d
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(n) = 0 for n < 1 */
    if (n < 1)
        return 0;

    seq_t sum = 0;
    idx_t d;

    /* iterate over divisors up to sqrt(n) */
    for (d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* first divisor d */
        idx_t mod = d % 12U;
        if (mod == 0 || mod == 1 || mod == 11)
            sum += (seq_t)d;

        /* paired divisor n/d, avoid double‑counting of square root */
        idx_t q = n / d;
        if (q != d) {
            mod = q % 12U;
            if (mod == 0 || mod == 1 || mod == 11)
                sum += (seq_t)q;
        }
    }

    return sum;
}
