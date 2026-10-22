/*
 * A385113  a(1) = 0, a(n) = 1 + 2*[omega(n) > 1] + [bigomega(n) > omega(n)],
 *          Iverson brackets, where omega = A001221 and bigomega = A001222.
 * https://oeis.org/A385113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 - 2*[n is a prime power] - [n is squarefree]; a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    /* factor_u64 will fill pr[] and ex[] with the distinct primes
       and their exponents of n, and cnt will be the number of
       distinct prime factors. */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    bool sqfree = true;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (ex[i] > 1)
        {
            sqfree = false;
            break;
        }
    }

    seq_t res;
    if (cnt == 1)          /* n is a prime power */
    {
        /* If the exponent is 1, n is prime; otherwise it is a
           proper prime power. */
        res = (ex[0] == 1) ? 1 : 2;
    }
    else                   /* composite with >1 distinct prime factors */
    {
        res = sqfree ? 3 : 4;
    }

    return res;
}
