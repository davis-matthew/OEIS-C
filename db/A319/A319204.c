/*
 * A319204  Sequence used for the Boas-Buck type recurrence for Riordan
 *          triangle A319203.
 * https://oeis.org/A319204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{2e2+3e3 = n+1} (-1)^(e2+e3) * (n+1)! / *
 *            ((n+1-(e2+e3))! e2! e3!) for n ≥ 0
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 26, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the multiplier identity (n+1)!/(…)=C(n+1,e2)*C(n+1-e2,e3) */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t n1 = n + (idx_t)1;          /* n+1 */
    seq_t sum = 0;

    for (idx_t e3 = 0; 3 * e3 <= n1; ++e3) {
        idx_t rem = n1 - 3 * e3;
        if (rem & 1) continue;      /* Must be even to give integer e2 */
        idx_t e2 = rem >> 1;        /* e2 = rem / 2 */

        /* term = C(n1, e2) * C(n1 - e2, e3) */
        seq_t term = binomial(n1, e2);
        term = term * binomial(n1 - e2, e3);

        /* apply sign factor */
        if ((e2 + e3) & 1)
            term = -term;

        sum += term;
    }
    return sum; /* a(0) is correctly 0 (empty sum) */
}
