/*
 * A351365  a(n) = Sum_{p|n, p prime} n^p.
 * https://oeis.org/A351365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} n^p
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 344 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ipow.h"

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
            sum += ipow((seq_t)n, (seq_t)p);
            /* remove all powers of p */
            while (nn % p == 0)
                nn /= p;
        }
    }

    /* if a prime factor > sqrt(original n) remains */
    if (nn > 1)
    {
        sum += ipow((seq_t)n, (seq_t)nn);
    }

    return sum;
}
