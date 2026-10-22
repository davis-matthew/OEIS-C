/*
 * A392630  The number of exponential divisors of n that are numbers whose
 *          number of divisors is a power of 2.
 * https://oeis.org/A392630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product over prime powers p^e in n of the * number of
 *            divisors f of e with f > 0 and f + 1 a power of 2. * This counts
 *            exponential divisors d of n (excluding 1) whose * number of
 *            divisors is a power of 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)            /* a(1) = 1 */
        return (seq_t)1;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        seq_t count = (seq_t)0;

        /* Count divisors f of e with f > 0 and f+1 a power of 2 */
        for (uint64_t f = 1; f <= e; ++f)
        {
            if (e % f == 0)          /* f divides e */
            {
                uint64_t fp1 = f + 1;
                if ((fp1 & (fp1 - 1)) == 0)   /* fp1 is a power of 2 */
                    ++count;
            }
        }
        result *= count;
    }

    return result;
}
