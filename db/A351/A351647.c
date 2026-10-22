/*
 * A351647  Sum of the squares of the odd proper divisors of n.
 * https://oeis.org/A351647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d<n, d odd} d^2 (OEIS A351647)
 * Range:     u8 n <= 29, u16 n <= 485, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* sum of squares of all odd proper divisors of n */
    seq_t sum = 0;
    for (idx_t d = 1; d < n; d += 2) {
        if (n % d == 0) {
            seq_t d2 = (seq_t)d * (seq_t)d;
            sum += d2;
        }
    }
    return sum;
}
