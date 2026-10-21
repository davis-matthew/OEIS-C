/*
 * A244049  Sum of all proper divisors of all positive integers <= n.
 * https://oeis.org/A244049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d=2}^{floor(n/2)} d * (floor(n/d)-1)
 * Range:     u8 n <= 29, u16 n <= 452, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes OEIS sequence A244049: Sum of all proper divisors of all
 * positive integers <= n.  The proper divisor set of m excludes 1 and m.
 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_d = n / 2;   /* floor(n/2) */
    for (idx_t d = 2; d <= max_d; ++d)
    {
        idx_t q = n / d;           /* number of multiples of d up to n */
        if (q > 1)                 /* exclude the multiple k=1 (m=d)    */
            sum += (seq_t)d * (seq_t)(q - 1);
    }
    return sum;
}
