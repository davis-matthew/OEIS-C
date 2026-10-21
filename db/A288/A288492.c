/*
 * A288492  Indices of terms of A288349 that are powers of 2.
 * https://oeis.org/A288492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 - 5*2^(n-2) + 2^(2*n-3) + n for n > 2; a(1)=1, a(2)=2,
 *            a(3)=3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 3;
    seq_t p1 = pow2(2*n - 3);      /* 2^(2n-3) */
    seq_t p2 = (seq_t)n + 2;       /* n + 2 */
    seq_t sum = p1 + p2;           /* part1 + n + 2 */
    seq_t p3 = pow2(n - 2);        /* 2^(n-2) */
    seq_t sub = 5 * p3;            /* 5*2^(n-2) */
    return sum - sub;
}
