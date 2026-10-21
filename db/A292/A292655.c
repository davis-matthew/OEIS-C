/*
 * A292655  Rank of n*e when all the numbers h, j*e, k*Pi, for h>=1, j>=1,
 *          k>=1, are jointly ranked. T.
 * https://oeis.org/A292655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*e) + floor(n*e/π) + n
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n) {
    /* constants: e and π computed once */
    static const long double E = expl(1.0L);
    static const long double PI = acosl(-1.0L);

    /* convert index to long double for accurate multiplication */
    long double ln = (long double)n;

    /* floor(n*e) */
    unsigned long long fe = (unsigned long long)floorl(ln * E + 1e-12L);

    /* floor(n*e/π) */
    unsigned long long fepi = (unsigned long long)floorl(ln * E / PI + 1e-12L);

    /* compute result in seq_t (promote to this type) */
    seq_t result = (seq_t)fe + (seq_t)fepi + (seq_t)n;
    return result;
}
