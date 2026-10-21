/*
 * A266297  Numbers whose last digit is a square.
 * https://oeis.org/A266297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 10*floor((n-1)/4) + [0,1,4,9][(n-1)%4]
 * Range:     u8 n <= 103, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
        /* n >= 1 per OEIS offset; behaviour for n < 1 is undefined */
        seq_t n_minus1 = (seq_t)n - 1;           /* (n-1) fits in seq_t */
        seq_t q = n_minus1 / 4;                 /* floor((n-1)/4)   */
        seq_t r = n_minus1 % 4;                 /* (n-1) mod 4      */
        static const unsigned int offs[4] = {0, 1, 4, 9};
        seq_t base = q * 10;                    /* 10 * q           */
        return base + (seq_t)(offs[(size_t)r]);
}
