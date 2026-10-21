/*
 * A290739  a(n) = 0 unless n = 3j^2+2j or 3j^2+4j+1 for some j>=0, in which
 *          case a(n) = (-1)^(j+1).
 * https://oeis.org/A290739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 unless n = 3*j*j + 2*j or n = 3*j*j + 4*j + 1 (j ≥ 0),
 *            in which case a(n) = (-1)^(j+1)
 * Range:     i8 n <= 4, i16 n <= 1208, i32 n <= 9127, i64 n <= 9127,
 *            i128 n <= 9127, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Compute term a(n) of OEIS sequence A290739. */
seq_t a(idx_t n)
{
    /* Convert index to the generic integer type. */
    seq_t nn = (seq_t)n;

    /* Discriminant D = 4 + 12*n.  Compute using seq_t to avoid overflow
       for builds where seq_t is a 64‑bit signed integer. */
    seq_t D = 4 + (seq_t)12 * nn;

    /* sqrt_D = floor(sqrt(D)).  This will be exact if D is a perfect square. */
    seq_t sqrtD = floor_sqrt(D);

    /* 0 if D is not a perfect square. */
    if (sqrtD * sqrtD != D)
        return 0;

    /* Check the first form: n = 3*j*j + 2*j  ==>  j = (sqrtD - 2)/6 */
    if (sqrtD >= 2 && ((sqrtD - 2) % 6) == 0) {
        seq_t j = (sqrtD - 2) / 6;
        seq_t n1 = (seq_t)3 * j * j + (seq_t)2 * j;
        if (n1 == nn)
            return ((j & 1) == 0) ? -1 : 1;   /* j even -> -1, j odd -> 1 */
    }

    /* Check the second form: n = 3*j*j + 4*j + 1  ==>  j = (sqrtD - 4)/6 */
    if (sqrtD >= 4 && ((sqrtD - 4) % 6) == 0) {
        seq_t j = (sqrtD - 4) / 6;
        seq_t n2 = (seq_t)3 * j * j + (seq_t)4 * j + 1;
        if (n2 == nn)
            return ((j & 1) == 0) ? -1 : 1;   /* j even -> -1, j odd -> 1 */
    }

    return 0;   /* n does not match either quadratic form. */
}
