/*
 * A325050  a(n) = Product_{k=0..n} (k!^2 + 1).
 * https://oeis.org/A325050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k!^2 + 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t res = 1;
    seq_t fact = 1;                 /* factorial accumulator  */
    for (idx_t k = 0; k <= n; ++k)
    {
        if (k != 0)
            fact *= (seq_t)k;       /* fact = k!   */
        seq_t term = fact * fact + 1;   /* k!^2 + 1   */
        res *= term;                 /* running product */
    }
    return res;
}
