/*
 * A361040  a(n) = 420*(3*n)!/(n!*(2*n + 3)!).
 * https://oeis.org/A361040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 70*binomial(3*n,2*n) - 189*binomial(3*n,2*n+1) * +
 *            114*binomial(3*n,2*n+2) - 32*binomial(3*n,2*n+3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Compute nth term of OEIS sequence A361040 */
seq_t a(idx_t n) {
    idx_t t   = 3 * n;          /* 3*n  */
    idx_t k2  = 2 * n;          /* 2*n  */

    seq_t b0 = binomial(t, k2);           /* C(3n,2n)   */
    seq_t b1 = binomial(t, k2 + 1);       /* C(3n,2n+1) */
    seq_t b2 = binomial(t, k2 + 2);       /* C(3n,2n+2) */
    seq_t b3 = binomial(t, k2 + 3);       /* C(3n,2n+3) */

    seq_t res = (seq_t)70 * b0
              - (seq_t)189 * b1
              + (seq_t)114 * b2
              - (seq_t)32  * b3;

    return res;
}
