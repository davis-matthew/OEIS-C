/*
 * A357912  a(n) = Sum_{d|n, d==1 (mod 11)} d.
 * https://oeis.org/A357912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d == 1 (mod 11)} d
 * Range:     u8 n <= 264, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of all positive divisors of n that are congruent to 1 modulo 11. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over divisor pairs (i, n/i) */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (d1 % 11 == 1) {
                sum += (seq_t)d1;
            }
            if (d2 != d1 && d2 % 11 == 1) {
                sum += (seq_t)d2;
            }
        }
    }
    return sum;
}
