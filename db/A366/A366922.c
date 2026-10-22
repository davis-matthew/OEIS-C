/*
 * A366922  a(n) is the exponent of 3 in the prime factorization of 10^n - 1.
 * https://oeis.org/A366922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = A007949(n) + 2 = v3(n) + 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute v_3(n), the exponent of 3 dividing n */
    idx_t tmp = n;
    seq_t v = 0;
    while (tmp % 3 == 0)
    {
        v++;
        tmp /= 3;
    }
    /* a(n) = v + 2 */
    return v + 2;
}
