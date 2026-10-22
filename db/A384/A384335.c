/*
 * A384335  Expansion of g.f.: cosh(7*arctanh(6*sqrt(x))).
 * https://oeis.org/A384335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^n * (512*n^3 + 64*n^2 + 144*n + 15) * C(2n,n) / 15 *
 *            where C(2n,n) = (2n)!/(n!)^2 is the central binomial
 *            coefficient. * This implementation uses elementary arithmetic on
 *            seq_t and * computes the values directly for any n >= 0
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* cast n to seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* compute 9^n */
    seq_t pow9 = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow9 *= (seq_t)9;

    /* compute central binomial coefficient C(2n,n) */
    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        binom *= (sn + (seq_t)i); /* multiply by (n + i) */
        binom /= (seq_t)i;        /* exact division by i */
    }

    /* compute polynomial part: 512*n^3 + 64*n^2 + 144*n + 15 */
    seq_t poly = (seq_t)512 * sn * sn * sn
               + (seq_t)64  * sn * sn
               + (seq_t)144 * sn
               + (seq_t)15;

    /* combine all parts and divide by 15 */
    seq_t result = pow9 * poly;
    result *= binom;
    result /= (seq_t)15;

    return result;
}
