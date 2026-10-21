/*
 * A293410  Least integer k such that k/n^2 > sqrt(3).
 * https://oeis.org/A293410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(sqrt(3)*n^2)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 114, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t t = (seq_t)n;
    seq_t n4 = t * t * t * t;        /* n^4 */
    seq_t prod = (seq_t)3 * n4;      /* 3 * n^4 */
    seq_t k = floor_sqrt(prod);      /* floor(sqrt(3)*n^2) */
    return k + 1;
}
