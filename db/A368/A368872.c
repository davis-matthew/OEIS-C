/*
 * A368872  Number of compositions (ordered partitions) of n into prime parts
 *          not greater than sqrt(n).
 * https://oeis.org/A368872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 33, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A368872
   a(n) = number of compositions of n into prime parts not greater than sqrt(n)
   The empty composition counts for n = 0. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* integer square root of a 64‑bit unsigned integer */
static idx_t floor_isqrt(idx_t n)
{
    idx_t lo = 0, hi = n >> 1 | 1;      /* hi > sqrt(n) */
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)            /* mid*mid <= n, avoiding overflow */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* sieve of Eratosthenes – returns the list of primes <= limit. */
static void sieve_primes(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2)
    {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    bool *is_prime = (bool *)malloc((size_t)(limit + 1));
    if (!is_prime)
    {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    for (size_t i = 0; i <= (size_t)limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;

    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++count;

    uint64_t *primes = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!primes)
    {
        free(is_prime);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[idx++] = (uint64_t)i;

    free(is_prime);
    *primes_out = primes;
    *cnt_out = count;
}

seq_t a(idx_t n)
{
    /* compute the maximum prime part: floor(sqrt(n)) */
    idx_t limit = floor_isqrt(n);

    /* generate all primes <= limit */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    sieve_primes(limit, &primes, &primes_cnt);

    /* dynamic programming array for compositions */
    seq_t *dp;
#if defined(__cplusplus)
    dp = new seq_t[(size_t)(n + 1)];
#else
    dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
#endif
    if (!dp)
    {
        free(primes);
#if defined(__cplusplus)
        return (seq_t)0;
#else
        return (seq_t)0;
#endif
    }

    dp[0] = (seq_t)1;                /* empty composition for n = 0 */

    for (idx_t sum = 1; sum <= n; ++sum)
    {
        seq_t ways = (seq_t)0;
        for (size_t i = 0; i < primes_cnt; ++i)
        {
            idx_t p = (idx_t)primes[i];
            if (p > sum)
                break;
            ways += dp[sum - p];
        }
        dp[sum] = ways;
    }

    seq_t result = dp[(size_t)n];

#if defined(__cplusplus)
    delete [] dp;
#else
    free(dp);
#endif
    free(primes);
    return result;
}
