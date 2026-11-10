/*
 * A303591  a(n) = 0 if n in A303589, 1 if n is in A303590.
 * https://oeis.org/A303591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n+1 = floor(k*alpha) for some k >= 1, else 1, where
 *            alpha = [1;1,2,3,4,...]. A303589 and A303590 are complementary
 *            Beatty sequences, so this equals floor((n+2)/alpha) -
 *            floor((n+1)/alpha) == 1 ? 0 : 1. 1/alpha is taken from a
 *            convergent of the continued fraction
 * Range:     u8 n <= 6, u16 n <= 98, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Validated: partial, pass, 107 terms through n = 106 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n + 1;
    seq_t lim = N * N + 2;
    seq_t pp = 1, qp = 0;   /* previous convergent */
    seq_t p = 1, q = 1;     /* current convergent [1] */
    seq_t i = 1;
    while (q <= lim) {
        seq_t np = i * p + pp;
        seq_t nq = i * q + qp;
        pp = p; qp = q;
        p = np; q = nq;
        i++;
    }
    /* 1/alpha ~ q/p */
    seq_t f1 = ((N + 1) * q) / p;
    seq_t f0 = (N * q) / p;
    return (f1 - f0 == 1) ? (seq_t)0 : (seq_t)1;
}
