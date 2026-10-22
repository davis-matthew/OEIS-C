/*
 * A375391  a(n) is the greatest odd number k such that n^2 + j is a semiprime
 *          for all odd numbers j from 1 to k.
 * https://oeis.org/A375391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 1950, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS A375391:  
 *  a(n) = the greatest odd k ≤ 15 such that for all odd j = 1,3,…,k  
 *         n*n + j is a semiprime; a(n) = -1 if n*n + 1 is not a semiprime. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility headers */
#include "../utils/build_primes_upto.h"   /* build_primes_upto(...) */
#include "../utils/is_prime_small.h"      /* is_prime_small(seq_t) */

/* -------------  static data and helpers ------------- */

static bool primes_init_done = false;
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* initialise the prime table once */
static void init_primes(void)
{
    if (primes_init_done) return;
    /* choose a limit that covers all numbers needed for the tests.
       n up to 1e4 gives n^2+15 < 1e8, whose square‑root is < 1e4.  
       We use a generous upper bound of 1e6 to be safe. */
    idx_t limit = 1000000;
    build_primes_upto(limit, &prime_list, &prime_count);
    primes_init_done = true;
}

/*  test whether m is a semiprime (product of exactly two primes,
    allowing the primes to be equal)                                   */
static bool is_semiprime(uint64_t m)
{
    if (m < 4) return false;                     /* 1,2,3 are not semiprime */

    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t p = prime_list[i];
        if (p > m / p) break;                   /* p*p > m  -> stop */
        if (m % p == 0)                         /* found a divisor */
        {
            uint64_t q = m / p;
            return is_prime_small((seq_t)q);    /* second factor must be prime */
        }
    }
    return false;                               /* m is prime or no factor found */
}

/* -------------  sequence function ------------- */

seq_t a(idx_t n)
{
    init_primes();                               /* ensure primes ready */

    /* compute n^2; 128‑bit to avoid overflow for very large n */
    __uint128_t n_sq = (__uint128_t)n * (__uint128_t)n;

    /* test odd offsets j = 1,3,…,15 */
    for (int j = 1; j <= 15; j += 2)
    {
        __uint128_t val128 = n_sq + j;
        if (val128 > UINT64_MAX)                /* cannot test beyond 64‑bit */
            return (seq_t)-1;
        uint64_t val = (uint64_t)val128;

        if (!is_semiprime(val))
        {
            if (j == 1) return (seq_t)-1;      /* first check failed */
            return (seq_t)(j - 2);             /* largest odd k before failure */
        }
    }
    return (seq_t)15;                            /* all eight checks succeeded */
}
