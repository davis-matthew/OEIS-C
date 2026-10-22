/*
 * A354983  a(n) = 1 if n is thrice a prime power congruent to 1 (mod 3),
 *          otherwise 0.
 * https://oeis.org/A354983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is thrice a prime power congruent to 1 (mod 3),
 *            otherwise 0
 * Range:     u8 n <= 9925, u16 n <= 9925, u32 n <= 9925, u64 n <= 9925,
 *            u128 n <= 9925, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n % 3 != 0)
        return 0;
    idx_t t = n / 3;
    if (t % 3 != 1)
        return 0;
    if (t <= 1)
        return 0;
    uint64_t k = (uint64_t)t;
    for (uint64_t d = 2; d <= k / d; ++d)
    {
        if (k % d == 0)
        {
            while (k % d == 0)
                k /= d;
            return k == 1;   /* all factors are the same prime */
        }
    }
    return 1;               /* k is prime itself */
}
