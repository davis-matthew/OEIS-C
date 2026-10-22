/*
 * A368528  a(n) = Sum_{k=1..n} k^2 * 3^(n-k).
 * https://oeis.org/A368528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(n+1) - (n^2 + 3*n + 3))/2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t power = pow3(n + 1);          /* 3^(n+1) */
    seq_t n_sq  = (seq_t)n * (seq_t)n;  /* n^2 */
    seq_t term  = n_sq + (seq_t)3 * (seq_t)n + (seq_t)3; /* n^2 + 3n + 3 */
    return (power - term) / 2;
}
