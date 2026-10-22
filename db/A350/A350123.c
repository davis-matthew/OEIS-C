/*
 * A350123  a(n) = Sum_{k=1..n} k^2 * floor(n/k)^2.
 * https://oeis.org/A350123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^2 * floor(n/k)^2
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 1834, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;                     /* floor(n/k) */
        seq_t k2 = (seq_t)k * (seq_t)k;      /* k^2 */
        seq_t q2 = (seq_t)q * (seq_t)q;      /* floor(n/k)^2 */
        sum += k2 * q2;
    }
    return sum;
}
