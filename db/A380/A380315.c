/*
 * A380315  Denominator of sum of reciprocals of all prime divisors of all
 *          positive integers <= n.
 * https://oeis.org/A380315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = product of all primes p ≤ n such that (n/p) mod p ≠ 0 *
 *            (that is the denominator of Σ_{p ≤ n} ⌊n/p⌋ / p in lowest terms)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Static storage for the list of primes */
static idx_t      prime_limit = 0;   /* largest value for which primes are known */
static uint64_t  *prime_list  = NULL;/* array of primes  (≤ prime_limit)       */
static size_t    prime_count = 0;    /* number of primes stored             */

/* Build a prime sieve up to 'limit' and store the primes in 'prime_list' */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;                      /* primes already sufficient */

    /* Allocate a sieve buffer */
    size_t max = (size_t)limit;
    if (max < 2)
        max = 2;
    bool *comp = (bool *)calloc(max + 1, sizeof(bool));
    if (!comp)
        return;                      /* allocation failure – leave primes unchanged */

    /* Simple Eratosthenes sieve */
    size_t cnt = 0;
    for (idx_t i = 2; i <= (idx_t)max; ++i)
    {
        if (!comp[i])
        {
            ++cnt;
            if ((size_t)i * (size_t)i <= max)
            {
                for (size_t j = (size_t)i * (size_t)i; j <= max; j += i)
                    comp[j] = true;
            }
        }
    }

    /* Store primes in an array */
    free(prime_list);
    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list)
    {
        free(comp);
        return;                      /* allocation failure – no primes stored */
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= (idx_t)max; ++i)
    {
        if (!comp[i])
            prime_list[idx++] = (uint64_t)i;
    }

    prime_count = cnt;
    prime_limit = limit;
    free(comp);
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n < 1)                /* offset error handling */
        return (seq_t)1;

    ensure_primes(n);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t p = prime_list[i];
        if (p > (uint64_t)n)
            break;

        idx_t k = n / (idx_t)p;
        if (k % (idx_t)p != 0)
            prod *= (seq_t)p;
    }
    return prod;
}
