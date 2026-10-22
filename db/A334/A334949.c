/*
 * A334949  a(n) is the total number of parts in all partitions of n into
 *          consecutive parts that differ by 6.
 * https://oeis.org/A334949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k | n, n > 3k(k-1)} k
 * Range:     u8 n <= 3359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Partition of n into consecutive parts differing by 6 is unique for each divisor k of n
    satisfying n > 3k(k-1); the parts are a, a-6, ..., a-6(k-1) where
    a = n/k + 3(k-1).  Each such partition contributes k parts to the total. */

#include <math.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* integer square root of n */
    idx_t sq = (idx_t) sqrt((double)n);
    while ((sq+1)*(sq+1) <= n) ++sq;
    while (sq*sq > n) --sq;

    for (idx_t d = 1; d <= sq; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            if (n > (3*d*(d-1)))
                sum += d;

            /* complementary divisor n/d */
            idx_t e = n / d;
            if (e != d) {
                if (n > (3*e*(e-1)))
                    sum += e;
            }
        }
    }
    return sum;
}
