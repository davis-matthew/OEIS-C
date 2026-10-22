/*
 * A369752  Expansion of e.g.f. exp(1 - (1+x)^4).
 * https://oeis.org/A369752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(4*a(n-1) + 12*(n-1)*a(n-2) + 12*(n-1)*(n-2)*a(n-3) +
 *            4*(n-1)*(n-2)*(n-3)*a(n-4)) Initial values: a(0)=1, a(1)=-4,
 *            a(2)=4, a(3)=56
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-4;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)56;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)-4;  /* a(1) */
    seq_t a2 = (seq_t)4;   /* a(2) */
    seq_t a3 = (seq_t)56;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t term1 = (seq_t)4 * a3;
        seq_t term2 = (seq_t)12 * ((seq_t)(i - 1)) * a2;
        seq_t term3 = (seq_t)12 * ((seq_t)(i - 1)) * ((seq_t)(i - 2)) * a1;
        seq_t term4 = (seq_t)4 * ((seq_t)(i - 1)) * ((seq_t)(i - 2)) *
                      ((seq_t)(i - 3)) * a0;
        seq_t next = -(term1 + term2 + term3 + term4);

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
