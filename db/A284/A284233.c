/*
 * A284233  Sum of odd prime power divisors of n (not including 1).
 * https://oeis.org/A284233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of all odd prime power divisors of n (excluding 1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* n is an unsigned 64‑bit integer.  Return the sum of all
       divisors of n that are odd prime powers (> 2).  The sum is
       computed using only integer operations.  */
    seq_t sum = 0;
    uint64_t tmp = n;

    /* Remove all factors of 2; they never contribute to the sum. */
    while ((tmp & 1) == 0)
        tmp >>= 1;

    /* Factor the remaining odd part.  For each odd prime p^e that
       divides n, add sum_{k=1..e} p^k to the total. */
    while (tmp > 1)
    {
        uint64_t p = _smallest_prime_factor(tmp);
        uint64_t e = 0;

        /* Count the exponent of p in tmp. */
        while (tmp % p == 0)
        {
            tmp /= p;
            ++e;
        }

        /* Accumulate the geometric series p + p^2 + ... + p^e. */
        seq_t term = 0;
        seq_t pk   = (seq_t)p;
        for (uint64_t k = 1; k <= e; ++k)
        {
            term += pk;
            pk *= (seq_t)p;
        }
        sum += term;
    }

    return sum;
}
