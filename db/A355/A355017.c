/*
 * A355017  a(n) is the number of bases in 2..n in which the sum of the digits
 *          of n is prime.
 * https://oeis.org/A355017
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) counts the bases b with 2 ≤ b ≤ n for which the sum of the
 *            digits of n in base b is a prime number
 * Range:     u8 n <= 822, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool prime_small(unsigned int x)
{
    if (x < 2) return false;
    for (unsigned int d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    seq_t result = 0;
    for (idx_t b = 2; b <= n; ++b)
    {
        idx_t temp = n;
        unsigned int sum = 0;
        while (temp)
        {
            sum += (unsigned int)(temp % b);
            temp /= b;
        }
        if (prime_small(sum)) ++result;
    }
    return result;
}
