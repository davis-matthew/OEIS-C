/*
 * A338652  Number of divisors of n which are greater than 8.
 * https://oeis.org/A338652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(n) - #{d | n , d <= 8}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* d(n) is the total number of positive divisors of n */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; for safety handle n < 1 */
    if (n < 1) return 0;

    uint64_t N = (uint64_t)n;          /* input as 64‑bit value */
    seq_t total = 1;                   /* total divisor count */
    uint64_t m = N;                    /* remaining factor to be processed */

    /* multiplicative formula: for each prime power p^e, multiply by (e+1) */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            seq_t exp = 0;             /* exponent of prime p */
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            total = total * (exp + 1);
        }
    }
    if (m > 1) /* remaining prime factor */
    {
        total = total * 2;
    }

    /* count divisors of N that are <= 8 */
    seq_t small = 0;
    for (uint64_t d = 1; d <= 8 && d <= N; ++d)
    {
        if (N % d == 0)
            ++small;
    }

    /* number of divisors > 8 is the difference */
    if (total > small)
        return total - small;
    else
        return 0;
}
