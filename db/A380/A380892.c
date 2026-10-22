/*
 * A380892  Hexagonal numbers that are abundant.
 * https://oeis.org/A380892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth hexagonal number H_k = k(2k-1) that is abundant
 *            (i.e., its sum of proper divisors exceeds the number itself)
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*** Prime generation *********************************************************/
#define PRIME_LIMIT 2000000U  /* enough for the first 10 000 terms of the sequence */

static uint32_t *prime_list   = NULL;   /* array of primes up to PRIME_LIMIT   */
static size_t   prime_count   = 0;      /* number of primes in prime_list      */

static void init_primes(void)
{
    if (prime_list != NULL)   /* already initialized */
        return;

    bool *is_prime = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (is_prime == NULL)
        exit(EXIT_FAILURE);
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        is_prime[i] = true;

    for (uint32_t p = 2; (uint64_t)p * p <= PRIME_LIMIT; ++p)
    {
        if (is_prime[p])
            for (uint64_t m = (uint64_t)p * p; m <= PRIME_LIMIT; m += p)
                is_prime[(size_t)m] = false;
    }

    /* count primes */
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++prime_count;

    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    if (prime_list == NULL)
        exit(EXIT_FAILURE);

    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = i;

    free(is_prime);
}

/*** Helper: abundance test for 64‑bit numbers ******************************/
static bool is_abundant_u64(uint64_t n)
{
    if (n < 12)            /* 12 is the smallest abundant integer */
        return false;

    uint64_t temp = n;
    unsigned __int128 sigma = 1;   /* product of sums over prime powers */

    for (size_t i = 0; i < prime_count; ++i)
    {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > temp)
            break;
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            unsigned __int128 sum_p = 1;
            unsigned __int128 power   = 1;
            for (int k = 0; k < exp; ++k)
            {
                power *= p;
                sum_p += power;
            }
            sigma *= sum_p;
            if (sigma > (__int128)2 * n)   /* early stop */
                return true;
        }
    }
    if (temp > 1)  /* remaining prime factor with exponent 1 */
        sigma *= (1 + (unsigned __int128)temp);

    return sigma > (__int128)2 * n;
}

/*** Sequence retrieval ********************************************************/
seq_t a(idx_t n)
{
    /* Single global cache for the sequence elements */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static uint64_t next_k = 1;   /* next k (hexagonal index) to examine */

    /* Initialise structures on first call */
    if (cache == NULL)
    {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            exit(EXIT_FAILURE);
    }
    init_primes();

    /* Extend cache until we have at least n terms */
    while (cache_size < n)
    {
        /* Hexagonal number H_k = k(2k-1) */
        uint64_t k       = next_k;
        uint64_t hex64   = k * (2 * k - 1);   /* safe in 64 bits for n≤10 000 */
        seq_t hex        = (seq_t)hex64;

        if (is_abundant_u64(hex64))
        {
            /* Grow cache if necessary */
            if (cache_size >= cache_cap)
            {
                cache_cap = cache_cap * 2 + 8;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (cache == NULL)
                    exit(EXIT_FAILURE);
            }
            cache[cache_size++] = hex;
        }
        ++next_k;
    }

    return cache[n - 1];
}
