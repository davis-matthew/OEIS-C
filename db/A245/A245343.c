/*
 * A245343  Sum of digits of n written in fractional base 5/3.
 * https://oeis.org/A245343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of digits of n in base 5/3 * (equivalently a(n) =
 *            A007953(A024633(n)) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t x   = n;

    while (x > 0) {
        unsigned d = x % 5;                  /* least–significant digit 0..4 */
        sum += d;

        /* compute next n: n' = (3*(x - d))/5
         * use 128‑bit integer to avoid overflow of 3*x   */
        unsigned __int128 prod = (unsigned __int128)(x - d) * 3;
        x = (idx_t)(prod / 5);
    }
    return sum;
}
