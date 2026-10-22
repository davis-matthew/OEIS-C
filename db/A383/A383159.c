/*
 * A383159  The sum of the maximum exponents in the prime factorizations of
 *          the unitary divisors of n.
 * https://oeis.org/A383159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n, gcd(d, n/d)=1}
 *            max_exponent_in_factorization_of(d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    uint64_t tmp = n;
    /* store exponents of prime factorization of n */
    int exps[64];          /* enough for any 64‑bit number */
    size_t k = 0;          /* number of distinct primes */

    /* trial division up to sqrt(n) – fast enough for n up to 10⁴ */
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            exps[k++] = e;
        }
    }
    if (tmp > 1)          /* remaining prime factor with exponent 1 */
        exps[k++] = 1;

    seq_t sum = 0;
    /* enumerate unitary divisors: each prime is either absent or taken whole */
    uint64_t subsets = 1ULL << k;           /* safe: k <= 64, but k is tiny (≤5 for n≤10000) */
    for (uint64_t mask = 0; mask < subsets; ++mask)
    {
        int mx = 0;
        for (size_t i = 0; i < k; ++i)
        {
            if (mask & (1ULL << i))
            {
                if (exps[i] > mx)
                    mx = exps[i];
            }
        }
        sum += mx;
    }
    return sum;
}
