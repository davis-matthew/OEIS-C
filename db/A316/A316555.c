/*
 * A316555  Number of distinct GCDs of nonempty submultisets of the integer
 *          partition with Heinz number n.
 * https://oeis.org/A316555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8719, u16 n <= 8719, u32 n <= 8719, u64 n <= 8719,
 *            u128 n <= 8719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of distinct gcds of nonempty subsets
 *              of the multiset consisting of the prime‑indices of the
 *              prime factors of n (the Heinz partition of n).
 *
 * The implementation is intended for n up to a few tens of thousands.
 * It uses a simple sieve to obtain all primes up to a fixed limit
 * and a small trial division routine for factorisation.
 * Distinct gcds are enumerated by brute force, which is trivial
 * because the number of distinct prime factors of n ≤ 10 000 is ≤ 5.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*----  Sieve  ----*/
#define MAX_PRIME_LIMIT 100000      /* > 10000 and enough for all n we care about */

static uint32_t *primes = NULL;     /* array of primes (1‑based) */
static size_t prime_count = 0;      /* number of primes found */
static uint16_t *prime_index = NULL;/* prime_index[p] = position of prime p, 1‑based */

/* Build primes up to MAX_PRIME_LIMIT and the index table.  */
static void init_primes(void)
{
    if (primes != NULL)
        return;                      /* already initialised */

    /* memory for primes: number of primes below 10^5 is < 9592 */
    primes = (uint32_t *)malloc((MAX_PRIME_LIMIT / 10) * sizeof(uint32_t));
    if (primes == NULL)
        return;                      /* in a real environment we would error */

    prime_index = (uint16_t *)calloc(MAX_PRIME_LIMIT + 1, sizeof(uint16_t));
    if (prime_index == NULL)
        return;

    bool *is_prime = (bool *)calloc(MAX_PRIME_LIMIT + 1, sizeof(bool));
    if (is_prime == NULL)
        return;

    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i)
        is_prime[i] = true;

    for (size_t p = 2; p * p <= MAX_PRIME_LIMIT; ++p)
    {
        if (is_prime[p])
        {
            for (size_t mult = p * p; mult <= MAX_PRIME_LIMIT; mult += p)
                is_prime[mult] = false;
        }
    }

    for (size_t p = 2; p <= MAX_PRIME_LIMIT; ++p)
    {
        if (is_prime[p])
        {
            prime_index[p] = (uint16_t)(prime_count + 1); /* 1‑based */
            primes[prime_count++] = (uint32_t)p;
        }
    }

    free(is_prime);
}

/*----  Helper: gcd for uint32_t  ----*/
static inline uint32_t gcd_u32(uint32_t a, uint32_t b)
{
    while (b != 0)
    {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/*----  Main sequence function  ----*/
seq_t a(idx_t n)
{
    if (n < 2)                    /* a(1) = 0 by definition */
        return 0;

    init_primes();

    /* factorise n: obtain distinct prime factors */
    uint32_t temp = (uint32_t)n;             /* n <= 10 000 in tests */
    uint32_t idxs[32];                       /* enough for any small n */
    size_t k = 0;

    for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= temp; ++i)
    {
        if (temp % primes[i] == 0)
        {
            /* prime factor found */
            idxs[k++] = prime_index[primes[i]]; /* 1‑based index of prime */
            while (temp % primes[i] == 0)
                temp /= primes[i];
        }
    }
    if (temp > 1)       /* remaining prime factor > sqrt(n) */
        idxs[k++] = prime_index[temp];

    if (k == 0)
        return 0;       /* safety, though n>=2 guarantees at least one factor */

    /* collect distinct gcds of all nonempty subsets of idxs */
    uint32_t uniq[64];
    size_t uniq_cnt = 0;

    /* iterate over all nonempty subsets */
    uint32_t max_mask = 1U << (uint32_t)k;
    for (uint32_t mask = 1; mask < max_mask; ++mask)
    {
        /* gcd of subset represented by mask */
        uint32_t g = 0;
        for (size_t j = 0; j < k; ++j)
        {
            if (mask & (1U << j))
            {
                g = (g == 0) ? idxs[j] : gcd_u32(g, idxs[j]);
            }
        }

        /* store unique gcd */
        bool found = false;
        for (size_t u = 0; u < uniq_cnt; ++u)
        {
            if (uniq[u] == g)
            {
                found = true;
                break;
            }
        }
        if (!found)
            uniq[uniq_cnt++] = g;
    }

    return (seq_t)uniq_cnt;
}
