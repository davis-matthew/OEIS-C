/*
 * A318457  a(n) = n XOR A001065(n), where XOR is bitwise-xor (A003987) and
 *          A001065 = sum of proper divisors.
 * https://oeis.org/A318457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n XOR sum of proper divisors of n
 * Range:     u8 n <= 143, u16 n <= 9868, u32 n <= 9868, u64 n <= 9868,
 *            u128 n <= 9868, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t xn = (seq_t)n;
    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            if (i != n)
                sum += (seq_t)i;                      /* divisor i, exclude n */
            idx_t j = n / i;
            if (j != i && j != n)
                sum += (seq_t)j;                      /* paired divisor, exclude n */
        }
    }
    return xn ^ sum;
}
