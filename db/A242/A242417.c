/*
 * A242417  Numbers in whose prime factorization both the first differences of
 *          indices of distinct primes and their exponents form a palindrome;
 *          fixed points of A242419.
 * https://oeis.org/A242417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 6942, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function brute‑forces the definition of OEIS A242417: for a given
   index n it enumerates integers m = 1,2,… and returns the nth integer
   whose prime factorisation has indices of primes whose successive
   differences and the exponents themselves both form palindromes. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------- prime tables --------------------------- */
#define PRIME_LIMIT        3000000U   /* covers all primes needed for n≈10000 */
#define MAX_PRIME_COUNT    250000U   /* upper bound on number of primes under LIMIT */

static uint32_t *prime_list   = NULL;   /* primes in order (1‑based index) */
static size_t   prime_count   = 0;
static uint32_t *prime_index  = NULL;   /* prime_index[p] = index of prime p (1‑based) */

/* initialise the prime tables once */
static void init_primes(void)
{
    static bool inited = false;
    if (inited) return;
    inited = true;

    prime_list  = (uint32_t*)malloc(sizeof(uint32_t) * MAX_PRIME_COUNT);
    prime_index = (uint32_t*)calloc((size_t)PRIME_LIMIT + 1, sizeof(uint32_t));

    uint8_t *sieve = (uint8_t*)calloc((size_t)PRIME_LIMIT + 1, sizeof(uint8_t));
    if (!sieve) return;            /* unlikely in contest environment */

    /* simple Sieve of Eratosthenes */
    for (size_t i = 0; i <= (size_t)PRIME_LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (uint64_t i = 2; i * i <= (uint64_t)PRIME_LIMIT; ++i)
    {
        if (sieve[i])
        {
            for (uint64_t j = i * i; j <= (uint64_t)PRIME_LIMIT; j += i)
                sieve[j] = 0;
        }
    }

    /* build list and index array */
    for (size_t i = 2; i <= (size_t)PRIME_LIMIT; ++i)
    {
        if (sieve[i])
        {
            prime_index[i] = (uint32_t)(prime_count + 1);   /* 1‑based index */
            prime_list[prime_count++] = (uint32_t)i;
        }
    }

    free(sieve);
}

/* ---------------------------- helper ------------------------------ */
static bool check_value(uint64_t n)
{
    if (n == 1) return true;   /* empty factorisation is a palindrome */

    uint32_t  primes[64];
    int       exps[64];
    size_t    cnt  = 0;
    uint64_t  temp = n;

    /* factorise using the pre‑computed prime list */
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > temp) break;
        if (temp % p == 0)
        {
            int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (temp > 1)   /* remaining prime factor > sqrt(n) */
    {
        primes[cnt] = (uint32_t)temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* indices of the primes (1‑based) */
    uint32_t idx[64];
    for (size_t i = 0; i < cnt; ++i)
    {
        uint32_t p = primes[i];
        if (p > PRIME_LIMIT) return false;   /* uniform safety guard */
        idx[i] = prime_index[p];
    }

    /* delta sequence of indices */
    int delta[64];
    delta[0] = (int)idx[0];
    for (size_t i = 1; i < cnt; ++i)
        delta[i] = (int)(idx[i] - idx[i - 1]);

    /* test palindrome property for deltas */
    for (size_t i = 0; i < cnt / 2; ++i)
        if (delta[i] != delta[cnt - 1 - i]) return false;

    /* test palindrome property for exponents */
    for (size_t i = 0; i < cnt / 2; ++i)
        if (exps[i] != exps[cnt - 1 - i]) return false;

    return true;
}

/* ----------------------------- main ------------------------------- */
seq_t a(idx_t n)
{
    static bool primes_inited = false;
    if (!primes_inited) { init_primes(); primes_inited = true; }

    uint64_t target = (uint64_t)n;
    uint64_t count  = 0;
    uint64_t m      = 1;

    while (true)
    {
        if (check_value(m))
        {
            ++count;
            if (count == target) return (seq_t)m;
        }
        ++m;
    }
}
