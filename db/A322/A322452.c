/*
 * A322452  Number of factorizations of n into factors > 1 not including any
 *          prime powers.
 * https://oeis.org/A322452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into factors > 1,
 *            none of which are prime powers. The factorization may contain a
 *            single factor n itself if n is not a prime power. Recursively
 *            enumerates factors in non‑decreasing order to avoid permutations
 * Range:     u8 n <= 9846, u16 n <= 9846, u32 n <= 9846, u64 n <= 9846,
 *            u128 n <= 9846, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static inline uint64_t isqrt64(uint64_t n)
{
    uint64_t r = (uint64_t)sqrt((double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

static bool is_prime_power(uint64_t x)
{
    if (x <= 1) return false;
    uint64_t t = x;
    int distinct = 0;
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            ++distinct;
            while (t % p == 0) t /= p;
            if (distinct > 1) return false;
        }
    }
    if (t > 1) ++distinct;          /* remaining prime factor */
    return distinct == 1;           /* true iff only one distinct prime factor */
}

static seq_t factorizations(uint64_t n, uint64_t min_factor)
{
    if (n == 1) return 1;            /* completed factorization */

    seq_t res = 0;

    /* count the factorization consisting solely of n */
    if (n >= min_factor && !is_prime_power(n))
        res += 1;

    uint64_t limit = isqrt64(n);
    for (uint64_t d = min_factor; d <= limit; ++d)
    {
        if (n % d == 0 && !is_prime_power(d))
            res += factorizations(n / d, d);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    return factorizations((uint64_t)n, 2);
}
