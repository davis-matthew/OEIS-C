/*
 * A349360  Number of positive integer pairs (s,t), with s,t <= n and s <= t
 *          such that either both s and t divide n or both do not.
 * https://oeis.org/A349360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = divisor_count(n)*(divisor_count(n)-n) + n*(n+1)/2 where
 *            divisor_count(n) is the number of positive divisors of n
 *            (A000005)
 * Range:     u8 n <= 24, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the divisor count of n (τ(n)) using trial division. */
static idx_t tau_of(idx_t n)
{
    idx_t count = 1;
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            idx_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            count *= (e + 1);
        }
    }
    if (n > 1)
        count *= 2;          /* n itself is a prime factor with exponent 1 */
    return count;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* τ(n) fits easily in a 64‑bit signed integer for the sizes we care about. */
    idx_t tau = tau_of(n);

    /* Compute the two parts of the formula in signed 64‑bit arithmetic. */
    long long part1 = (long long)tau * ((long long)tau - (long long)n);
    long long part2 = (long long)n * ((long long)n + 1) / 2;

    long long result = part2 + part1; /* guaranteed non‑negative for n ≥ 1 */

    return (seq_t)result;
}
