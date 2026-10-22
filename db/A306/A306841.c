/*
 * A306841  Number of rectangles of integer sides whose area or perimeter is
 *          n.
 * https://oeis.org/A306841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(d(n)/2) + floor(n/4) if n even, otherwise a(n) =
 *            ceil(d(n)/2)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Count divisors of n */
static seq_t divisor_count(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            if (i == n / i)
                ++cnt;
            else
                cnt += 2;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t d = divisor_count(n);
    seq_t res = (d + 1) / 2;          /* ceil(d/2) */
    if ((n & 1) == 0)                 /* n even */
        res += n / 4;                 /* floor(n/4) */
    return res;
}
