/*
 * A375039  The maximum exponent in the prime factorization of 2*n-1.
 * https://oeis.org/A375039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum exponent in the prime factorization of 2*n-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute the odd number 2*n-1 */
    uint64_t m = (uint64_t)n * 2 - 1;
    if (m <= 1)           /* n = 1 → 2*n-1 = 1 → no prime factors */
        return (seq_t)0;

    int max_exp = 0;
    /* m is odd; no need to test factor 2 */
    uint64_t p = 3;
    while (p * p <= m)
    {
        if (m % p == 0)
        {
            int cnt = 0;
            while (m % p == 0)
            {
                m /= p;
                cnt++;
            }
            if (cnt > max_exp)
                max_exp = cnt;
        }
        p += 2;   /* only odd divisors */
    }

    /* Any remaining m > 1 is a prime factor with exponent 1 */
    if (m > 1 && max_exp < 1)
        max_exp = 1;

    return (seq_t)max_exp;
}
