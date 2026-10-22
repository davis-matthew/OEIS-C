/*
 * A304024  a(n) is the largest integer with n digits in base 3/2.
 * https://oeis.org/A304024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Compute A304024 by recurrence * a(0) = 2 * for n>0: * if
 *            A304025(n-1) is even: a(n) = 10*a(n-1) + 2 * else a(n) =
 *            10*a(n-1) - 8 * * A304025(n) – the value of a(n) in base 10 –
 *            satisfies * A304025(0) = 2 * if A304025(m-1) is even: A304025(m)
 *            = (3*A304025(m-1))/2 + 2 * else A304025(m) = (3*A304025(...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)2;   /* A304025 value for previous index */
    seq_t res = (seq_t)2;   /* a(n) value for previous index */

    for (idx_t i = 1; i <= n; ++i)
    {
        if ((val & (seq_t)1) == (seq_t)0)         /* previous value even */
        {
            val = val * (seq_t)3 / (seq_t)2 + (seq_t)2;
            res = res * (seq_t)10 + (seq_t)2;
        }
        else                                      /* previous value odd */
        {
            val = (val * (seq_t)3 + (seq_t)1) / (seq_t)2;
            res = res * (seq_t)10 - (seq_t)8;
        }
    }
    return res;
}
