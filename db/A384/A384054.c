/*
 * A384054  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is an exponentially
 *          odd number.
 * https://oeis.org/A384054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n minus the count of numbers ≤ n that are divisible by at
 *            least one prime power p^e dividing n with an even exponent e.
 *            This is equivalent to counting k such that the greatest unitary
 *            divisor of k that divides n has all prime exponents odd
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"
#include "../utils/_floor_isqrt_u64.h"

static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

static void ensure_prime_list(idx_t limit)
{
    if (prime_list == NULL || limit > prime_limit)
    {
        free(prime_list);
        prime_list = NULL;
        if (limit < 2)
        {
            prime_limit = 0;
            prime_cnt = 0;
            return;
        }
        build_primes_upto(limit, &prime_list, &prime_cnt);
        prime_limit = limit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* factor n and collect prime powers with even exponent */
    uint64_t m = (uint64_t)n;
    seq_t even_pows[64];
    size_t even_cnt = 0;

    /* handle factor 2 first */
    if ((m & 1) == 0)
    {
        int exp = 0;
        while ((m & 1) == 0)
        {
            m >>= 1;
            exp++;
        }
        if ((exp & 1) == 0)   /* even exponent */
            even_pows[even_cnt++] = big_pow((seq_t)2, (seq_t)exp);
    }

    /* generate primes up to sqrt(n) if needed */
    idx_t limit = (n >= 2) ? _floor_isqrt_u64(n) : 1;
    ensure_prime_list(limit);

    /* trial division using the prime list */
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= m; ++i)
    {
        uint64_t p = prime_list[i];
        if (p == 2) continue;  /* already handled */
        if (m % p == 0)
        {
            int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                exp++;
            }
            if ((exp & 1) == 0)   /* even exponent */
                even_pows[even_cnt++] = big_pow((seq_t)p, (seq_t)exp);
        }
    }

    if (m > 1)   /* remaining prime factor with exponent 1 (odd) */
    {
        /* if it had an even exponent it would have not survived trial division */
        /* nothing to do */
    }

    seq_t N = (seq_t)n;
    if (even_cnt == 0)
        return N;

    seq_t excl = 0;
    size_t masks = ((size_t)1) << even_cnt;

    for (size_t mask = 1; mask < masks; ++mask)
    {
        seq_t prod = (seq_t)1;
        for (size_t j = 0; j < even_cnt; ++j)
        {
            if (mask & ((size_t)1 << j))
                prod = prod * even_pows[j];
        }
        seq_t term = N / prod;
        if (__builtin_popcountll(mask) & 1)
            excl += term;
        else
            excl -= term;
    }

    return N - excl;
}
