/*
 * A351365  a(n) = Sum_{p|n, p prime} n^p.
 * https://oeis.org/A351365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} n^p
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t nn = (uint64_t)n;
    seq_t sum = 0;

    /* factorize nn by trial division */
    for (uint64_t p = 2; p <= nn / p; ++p)
    {
        if (nn % p == 0)
        {
            sum += big_pow((seq_t)n, (seq_t)p);
            /* remove all powers of p */
            while (nn % p == 0)
                nn /= p;
        }
    }

    /* if a prime factor > sqrt(original n) remains */
    if (nn > 1)
    {
        sum += big_pow((seq_t)n, (seq_t)nn);
    }

    return sum;
}
