/*
 * A284117  Sum of proper prime power divisors of n.
 * https://oeis.org/A284117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{p^k | n, k >= 2} p^k (sum of proper prime‑power
 *            divisors of n)
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)                /* 1 and all primes have no prime‑power divisors with k>=2 */
        return 0;

    uint64_t m = (uint64_t)n; /* copy for factoring, avoid modifying the parameter */
    seq_t total = 0;

    /* trial division: factor m, computing the exponent of each prime factor */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p != 0)
            continue;

        uint64_t e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }

        if (e < 2)            /* only exponents >= 2 contribute a p^k to the sum */
            continue;

        /* add p^2 + p^3 + ... + p^e to total */
        uint64_t pow = (uint64_t)p * (uint64_t)p;   /* p^2 */
        for (uint64_t k = 2; k <= e; ++k)
        {
            total += (seq_t)pow;
            pow *= (uint64_t)p;
        }
    }

    /* If m > 1 here, m is a prime factor with exponent 1, contributing nothing. */
    return total;
}
