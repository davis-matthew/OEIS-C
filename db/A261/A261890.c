/*
 * A261890  Second differences of A055615, first differences of A261869.
 * https://oeis.org/A261890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n+2)*mu(n+2) - 2*(n+1)*mu(n+1) + n*mu(n)
 * Range:     i8 n <= 44, i16 n <= 8200, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* compute mu(n), mu(n+1), mu(n+2) */
    seq_t m0 = mobius(n);
    seq_t m1 = mobius(n + 1);
    seq_t m2 = mobius(n + 2);

    /* (n+2)*mu(n+2) */
    seq_t term2 = (seq_t)(n + 2) * m2;

    /* 2*(n+1)*mu(n+1) */
    seq_t term1 = 2 * (seq_t)(n + 1) * m1;

    /* n*mu(n) */
    seq_t term0 = (seq_t)n * m0;

    /* a(n) = term2 - term1 + term0 */
    return term2 - term1 + term0;
}
