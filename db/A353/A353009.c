/*
 * A353009  a(n) = Sum_{k=0..floor(n/2)} (n-2*k)^(n-2*k).
 * https://oeis.org/A353009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-2*k)^(n-2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t m = n; ; )
    {
        /* (m)^(m) with m = n-2*k */
        seq_t term = seq_pow((seq_t)m, m);
        sum += term;
        /* stop when the next step would make m negative */
        if (m < 2)
            break;
        m -= 2;
    }
    return sum;
}
