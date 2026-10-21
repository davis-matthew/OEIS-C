/*
 * A278403  a(n) = Sum_{d|n} d^2 * (d+1)/2.
 * https://oeis.org/A278403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d | n} d^2 * (d + 1) / 2 = (sigma_3(n) + sigma_2(n))
 *            / 2
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over divisors up to sqrt(n) */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t ds = (seq_t)d;
            seq_t part = ds * ds;          /* d^2 */
            part = part * (ds + 1);        /* d^2 * (d+1) */
            part /= 2;                    /* integer division by 2 */
            sum += part;

            idx_t q = n / d;
            if (q != d) {
                seq_t qs = (seq_t)q;
                seq_t part_q = qs * qs;    /* q^2 */
                part_q = part_q * (qs + 1);
                part_q /= 2;
                sum += part_q;
            }
        }
    }
    return sum;
}
