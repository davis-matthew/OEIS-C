/*
 * A353078  Inverse Moebius transform of odd primes.
 * https://oeis.org/A353078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} prime(d+1)
 * Range:     u8 n <= 29, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* defined for n ≥ 1 by OEIS, this is safe for lower offset */

    seq_t sum = 0;
    idx_t i;

    for (i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            sum += (seq_t)get_prime(i + 1);
            idx_t j = n / i;
            if (j != i)
                sum += (seq_t)get_prime(j + 1);
        }
    }

    return sum;
}
