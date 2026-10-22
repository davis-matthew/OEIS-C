/*
 * A320042  a(n) = a(floor(n/2)) + (-1)^(n*(n+1)/2) with a(1)=0.
 * https://oeis.org/A320042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, hear
 * Formula:   a(n) = a(floor(n/2)) + (-1)^(n*(n+1)/2), a(1) = 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;          /* base case a(1) = 0 (and a(0) unused) */

    seq_t sub = a(n >> 1);          /* a(floor(n/2)) */
    seq_t nn  = (seq_t)n;           /* promote n to seq_t for calculations */
    seq_t t   = (nn * (nn + 1)) / 2;/* triangular number T(n) = n*(n+1)/2 */
    seq_t sign = (t & 1) ? -1 : 1;  /* (-1)^T(n) : +1 if T(n) even, -1 otherwise */
    return sub + sign;
}
