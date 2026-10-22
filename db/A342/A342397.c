/*
 * A342397  Expansion of the o.g.f. (2*x^2 + 3*x + 2)*x/((x + 1)^2*(x - 1)^4).
 * https://oeis.org/A342397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(1 - (-1)^n)/16 + (7/4)*(n*(n+1)*(n+2)/6) *
 *            Simplified integer form: * If n is even: a(n) = (n+1) * 7 * n *
 *            (n+2) / 24 * If n is odd : a(n) = (n+1) * (7 * n * (n+2) + 3) /
 *            24 * * This avoids floating point and checks that the result is
 *            integral for all n
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t num;
    seq_t n_t = (seq_t)n;                 /* cast index to seq_t */
    seq_t term = (seq_t)7 * n_t * (seq_t)(n + 2); /* 7*n*(n+2) */

    if (n & 1) {                 /* n is odd */
        num = ((seq_t)(n + 1)) * (term + 3);
    } else {                     /* n is even */
        num = ((seq_t)(n + 1)) * term;
    }

    return num / 24;             /* division is exact */
}
