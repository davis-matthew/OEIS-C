/*
 * A352945  a(n) = Sum_{k=0..floor(n/3)} k^(n-3*k).
 * https://oeis.org/A352945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} k^(n-3*k)
 * Range:     u8 n <= 13, u16 n <= 18, u32 n <= 27, u64 n <= 42, u128 n <= 66,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t m = n / 3;            /* floor(n/3) */
    for (idx_t k = 0; k <= m; ++k)
    {
        idx_t e = n - 3 * k;    /* exponent n-3k */
        seq_t term;
        if (k == 0)
        {
            /* 0^0 is defined as 1; 0^e (e>0) is 0 */
            term = (e == 0 ? (seq_t)1 : (seq_t)0);
        }
        else
        {
            term = big_pow((seq_t)k, (seq_t)e);
        }
        sum += term;
    }
    return sum;
}
