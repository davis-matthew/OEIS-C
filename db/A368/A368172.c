/*
 * A368172  The number of divisors of the largest cubefull exponentially odd
 *          divisor of n (A368170).
 * https://oeis.org/A368172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = the number of divisors of the largest cubefull *
 *            exponentially odd divisor of n. * The sequence is multiplicative
 *            with * a(p^e) = 1 if e <= 2 * a(p^e) = e + 1 if e is odd and e >
 *            1 * a(p^e) = e otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   local_contribution
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/local_contribution.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */
    uint64_t temp = (uint64_t)n;
    seq_t res = (seq_t)1;

    /* factorisation by trial division */
    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            res *= local_contribution(e);
        }
    }
    /* remaining prime factor > sqrt(original n) */
    if (temp > 1)
    {
        /* exponent is 1 */
        res *= local_contribution(1);   /* local_contribution(1) == 1, so no effect */
    }

    return res;
}
