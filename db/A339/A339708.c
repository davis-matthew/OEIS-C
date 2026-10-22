/*
 * A339708  a(n) is the number of decompositions of 2*n as the sum of an odd
 *          prime and a semiprime.
 * https://oeis.org/A339708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 2024, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A339708: number of ways to write 2*n as odd prime + semiprime */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static idx_t    max_limit = 0;      /* maximum 2*n processed */
static uint64_t *prime_list = NULL;/* array of primes up to max_limit */
static size_t   prime_cnt   = 0;    /* number of primes stored */

/* build or extend the prime table up to at least 'limit' */
static void ensure_data(idx_t limit)
{
    if (limit <= max_limit)
        return;

    /* free old table if any */
    if (prime_list)
    {
        free(prime_list);
        prime_list = NULL;
    }

    /* build new table; prime_list[0 .. prime_cnt-1] will contain all primes <= limit */
    build_primes_upto(limit, &prime_list, &prime_cnt);
    max_limit = limit;
}

/* test whether x is a semiprime (product of exactly two primes) */
static bool is_semiprime(uint64_t x)
{
    if (x < 4)   /* smallest semiprime is 2*2 = 4 */
        return false;

    uint64_t rem   = x;
    uint64_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if (p * p > rem)
            break;

        while (rem % p == 0)
        {
            ++count;
            if (count > 2)
                return false;
            rem /= p;
        }
    }

    if (rem > 1)
        ++count;   /* remaining prime factor */

    return count == 2;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* offset starts at 1 */

    idx_t target = n * 2ULL;
    ensure_data(target);

    seq_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if (p == 2)            /* only odd primes are allowed */
            continue;

        if (p > target)        /* primes are sorted */
            break;

        uint64_t rest = target - p;
        if (rest <= 1)
            continue;

        if (is_semiprime(rest))
            ++count;
    }

    return count;
}
