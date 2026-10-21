/*
 * A294614  Sum of the divisors of (12*n - 1), divided by 12, minus n.
 * https://oeis.org/A294614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = sigma(12*n - 1)/12 - n
 * Range:     u8 n <= 17, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* m = 12*n - 1 */
    seq_t m = (seq_t)12 * n - 1;

    /* compute sigma(m) via prime factorisation */
    seq_t sigma = 1;
    seq_t temp = m;

    /* trial division up to sqrt(temp) */
    for (seq_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            seq_t power = p;
            seq_t sum = 1 + p;
            temp /= p;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
                sum += power;
            }
            sigma *= sum;
        }
    }

    /* remaining prime factor > 1 */
    if (temp > 1) {
        sigma *= (1 + temp);
    }

    /* a(n) = sigma / 12 - n */
    return sigma / 12 - n;
}
