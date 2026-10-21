/*
 * A252584  a(n) = a(n-1)^3 + a(n-2)^9, a(0) = 0, a(1) = 1.
 * https://oeis.org/A252584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^3 + a(n-2)^9 Initial values: a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)0;          /* a(n-2) */
    seq_t a1 = (seq_t)1;          /* a(n-1) */
    seq_t ai;                     /* a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        ai = seq_pow(a1, 3) + seq_pow(a0, 9);
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
