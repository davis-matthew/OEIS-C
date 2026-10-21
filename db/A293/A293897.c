/*
 * A293897  Sum of proper divisors of n of the form 3k+1.
 * https://oeis.org/A293897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of proper divisors of n that are ≡ 1 (mod 3)
 * Range:     u8 n <= 319, u16 n <= 8913, u32 n <= 8913, u64 n <= 8913,
 *            u128 n <= 8913, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            if (d1 < n && d1 % 3 == 1)
            {
                sum += (seq_t)d1;
            }
            idx_t d2 = n / i;
            if (d2 != d1 && d2 < n && d2 % 3 == 1)
            {
                sum += (seq_t)d2;
            }
        }
    }
    return sum;
}
