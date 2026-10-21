/*
 * A273190  a(n) is the number of nonnegative m < n for which m + n is a
 *          perfect square.
 * https://oeis.org/A273190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt(2*n-1)) - floor(sqrt(n-1)) for n>0, a(0) = 0
 * Range:     u8 n <= 15, u16 n <= 1455, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor_sqrt(x) for any unsigned seq_t.  */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low  = (seq_t)0;
    seq_t high = x;

    while (low < high)
    {
        /* mid = ceil((low+high)/2) – use +1 before shift to round up */
        seq_t mid = low + ((high - low + 1) >> 1);
        seq_t sq  = mid * mid;

        if (sq <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sn     = (seq_t)n;
    seq_t x1     = sn * (seq_t)2 - (seq_t)1;      /* 2*n - 1 */
    seq_t x2     = sn - (seq_t)1;                 /* n - 1  */

    seq_t s1     = isqrt_seq(x1);                 /* floor(sqrt(2*n-1)) */
    seq_t s2     = isqrt_seq(x2);                 /* floor(sqrt(n-1))   */

    return s1 - s2;
}
