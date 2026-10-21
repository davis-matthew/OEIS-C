/*
 * A295576  a(n) = Sum_{1 <= j <= n/2, gcd(j,n)=1} j^4.
 * https://oeis.org/A295576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{1 <= j <= n/2, gcd(j,n)=1} j^4
 * Range:     u8 n <= 8, u16 n <= 28, u32 n <= 238, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t nseq = (seq_t)n;
    idx_t half = n >> 1;
    for (idx_t j = 1; j <= half; ++j)
    {
        if (gcd((seq_t)j, nseq) == 1)
        {
            seq_t jseq = j;
            seq_t j2 = jseq * jseq;
            seq_t j4 = j2 * j2;
            sum += j4;
        }
    }
    return sum;
}
