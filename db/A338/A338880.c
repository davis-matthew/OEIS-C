/*
 * A338880  Product of the nonzero digits of (n written in base 7).
 * https://oeis.org/A338880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of nonzero base‑7 digits of n, with a(0)=1
 * Range:     u8 n <= 929, u16 n <= 9603, u32 n <= 9603, u64 n <= 9603,
 *            u128 n <= 9603, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    if (n == 0) return prod;          /* a(0) = 1 by definition */
    while (n > 0)
    {
        unsigned d = n % 7;
        if (d != 0)
            prod *= (seq_t)d;
        n /= 7;
    }
    return prod;
}
