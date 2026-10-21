/*
 * A293116  Expansion of e.g.f. exp(x/(x-1)).
 * https://oeis.org/A293116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=−1, a(n) = (2n−3)a(n−1) − (n−1)(n−2)a(n−2) for n≥2
 *            (D‑finite recurrence)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;

    seq_t a0 = (seq_t)1;     /* a(0) */
    seq_t a1 = (seq_t)-1;    /* a(1) */
    seq_t a2;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t factor1 = (seq_t)2 * (seq_t)i - (seq_t)3;               /* 2i−3 */
        seq_t factor2 = (seq_t)(i - 1) * (seq_t)(i - 2);               /* (i−1)(i−2) */
        a2 = factor1 * a1 - factor2 * a0;                              /* a(i) */
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
