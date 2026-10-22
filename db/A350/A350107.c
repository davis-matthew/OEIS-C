/*
 * A350107  a(n) = Sum_{k=1..n} k * floor(n/k)^2.
 * https://oeis.org/A350107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * floor(n/k)^2
 * Range:     u8 n <= 10, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = n / k;                       /* floor(n/k) as idx_t */
        seq_t tt = (seq_t)t * (seq_t)t;        /* (floor(n/k))^2 in seq_t */
        sum += (seq_t)k * tt;                  /* add k * (floor(n/k))^2   */
    }
    return sum;
}
