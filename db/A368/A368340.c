/*
 * A368340  Take the solution to Pellian equation x^2 - 8*n*y^2 = 1 with
 *          smallest positive y and x >= 0; sequence gives a(n) = x, or 1 if n
 *          is twice a positive square. A368339 gives values of y.
 * https://oeis.org/A368340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal non‑negative integer x such that x^2 – 8 n y^2 =
 *            1 where y>0 is the smallest possible. If n is twice a positive
 *            square a(n)=1. This is a(n)=A002350(8 n)
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* D = 8 * n  (must be computed in seq_t to avoid overflow) */
    seq_t D = (seq_t)8 * n;

    /* Perfect square test: n is twice a square iff D is a square. */
    seq_t r = floor_sqrt(D);
    if (r * r == D)            /* D is a perfect square → n = 2*k^2 */
        return (seq_t)1;

    /* Continued‑fraction method for the fundamental solution of
       x^2 – D y^2 = 1.  All arithmetic is done in seq_t. */
    seq_t a0 = r;          /* floor(sqrt(D)) */
    seq_t p_m2 = (seq_t)0, p_m1 = (seq_t)1;   /* p_{-2}, p_{-1} */
    seq_t q_m2 = (seq_t)1, q_m1 = (seq_t)0;   /* q_{-2}, q_{-1} */

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a = a0;

    while (1) {
        /* Compute next convergent p/q */
        seq_t p = a * p_m1 + p_m2;
        seq_t q = a * q_m1 + q_m2;

        /* Check Pell equation */
        seq_t lhs = p * p - D * q * q;
        if (lhs == (seq_t)1)
            return p;  /* minimal x found */

        /* Update for next term of the continued fraction */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        /* Shift previous convergents */
        p_m2 = p_m1;  p_m1 = p;
        q_m2 = q_m1;  q_m1 = q;
    }
}
