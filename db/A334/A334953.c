/*
 * A334953  a(n) is the sum of all parts of all partitions of n into
 *          consecutive parts that differ by 6.
 * https://oeis.org/A334953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (# of divisors k of n such that n/k ≥ 3*(k-1)+1).
 *            This counts the partitions of n into consecutive parts that
 *            differ by 6. The sum of all parts in these partitions is then n
 *            times the number of such partitions
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    idx_t sqrt_n = (idx_t)sqrt((double)n);
    seq_t count   = 0;

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            /* divisor d -> k = d */
            idx_t k = d;
            idx_t t = n / k;                     /* t = n/k */
            if (t >= 3 * (k - 1) + 1) ++count;

            /* paired divisor n/d -> k = n/d */
            idx_t k2 = n / d;
            if (k2 != k) {
                idx_t t2 = n / k2;               /* t2 == d */
                if (t2 >= 3 * (k2 - 1) + 1) ++count;
            }
        }
    }
    return (seq_t)n * count;
}
