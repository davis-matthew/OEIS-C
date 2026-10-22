/*
 * A300909  Sum of 4th powers dividing n.
 * https://oeis.org/A300909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum_{d|n, d is fourth power} d This implementation
 *            factors n and uses the multiplicative formula A(p^e) =
 *            (p^(4*(1+⌊e/4⌋)) – 1)/(p^4 – 1). Complexity: O(√n) per call,
 *            well under the limits for n up to at least 10^6
 * Range:     u8 n <= 15, u16 n <= 9097, u32 n <= 9097, u64 n <= 9097,
 *            u128 n <= 9097, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer power: res = base^exp */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1) res *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return res;
}

/* a(n) for OEIS A300909 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safety guard; offset is 1 */
    seq_t result = 1;
    uint64_t m = n;

    /* factor 2 separately */
    if ((m & 1) == 0)
    {
        idx_t e = 0;
        while ((m & 1) == 0)
        {
            m >>= 1;
            e++;
        }
        idx_t block = e / 4;
        if (block)
        {
            seq_t denom = pow_seq((seq_t)2, 4);
            denom -= 1;
            seq_t numer = pow_seq((seq_t)2, 4 * (block + 1));
            result *= numer / denom;
        }
    }

    /* odd prime factors */
    for (uint64_t i = 3; i <= m / i; i += 2)
    {
        if (m % i == 0)
        {
            idx_t e = 0;
            while (m % i == 0)
            {
                m /= i;
                e++;
            }
            idx_t block = e / 4;
            if (block)
            {
                seq_t denom = pow_seq((seq_t)i, 4);
                denom -= 1;
                seq_t numer = pow_seq((seq_t)i, 4 * (block + 1));
                result *= numer / denom;
            }
        }
    }

    /* remaining prime factor > sqrt(n) (exponent 1) – contributes nothing */
    return result;
}
