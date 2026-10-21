/*
 * A293901  Sum of proper divisors of n of the form 4k+1.
 * https://oeis.org/A293901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d<n} [1 == d mod 4]*d
 * Range:     u8 n <= 449, u16 n <= 8353, u32 n <= 8353, u64 n <= 8353,
 *            u128 n <= 8353, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t sum = 0;

    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            if (d < n && (d & 3) == 1)
                sum += (seq_t)d;

            idx_t q = n / d;
            if (q != d && q < n && (q & 3) == 1)
                sum += (seq_t)q;
        }
    }

    return sum;
}
