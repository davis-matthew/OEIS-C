/*
 * A295575  a(n) = Sum_{1 <= j <= n/2, gcd(j,n)=1} j^3.
 * https://oeis.org/A295575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1 <= j <= n/2, gcd(j,n)=1} j^3
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 726, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: direct summation checking coprimality with j and n. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard against 0 */

    seq_t sum = 0;
    idx_t half = n / 2;             /* integer division works for unsigned types */

    for (idx_t j = 1; j <= half; ++j)
    {
        if (gcd((seq_t)j, (seq_t)n) == 1)
        {
            seq_t v = (seq_t)j;
            sum += v * v * v;      /* v³ */
        }
    }
    return sum;
}
