/*
 * A292982  Bi-unitary abundant numbers: numbers k such that bsigma(k) > 2*k,
 *          where bsigma is the sum of the bi-unitary divisors function
 *          (A188999).
 * https://oeis.org/A292982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 3299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth bi‑unitary abundant number.
   The routine enumerates integers starting at 1 and keeps a
   cache of discovered terms.  It uses trial division by
   precomputed primes to evaluate the bi‑unitary divisor sum
   (bsigma) for each integer a and tests the inequality
   bsigma(k) > 2*k. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- prime generation ----------------------------------------------------- */
static uint32_t *prime_list = NULL;
static size_t prime_count  = 0;

static void init_primes(void)
{
    if (prime_list != NULL)
        return;

    /* fixed limit large enough for all expected queries */
    const size_t LIMIT = 2000000;               /* 2·10^6 */
    bool *is_prime = (bool*)malloc(LIMIT + 1);
    if (!is_prime) abort();

    for (size_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    /* count primes */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++prime_count;

    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            prime_list[idx++] = (uint32_t)i;

    free(is_prime);
}

/* --- bi‑unitary sigma for a single integer -------------------------------- */
static seq_t bsigma_value(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t  tmp   = n;
    seq_t  total = (seq_t)1;

    for (size_t pi = 0; pi < prime_count; ++pi)
    {
        uint32_t p = prime_list[pi];
        if ((uint64_t)p * p > tmp)
            break;
        if (tmp % p == 0)
        {
            idx_t exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }

            seq_t sum   = (seq_t)1;
            seq_t term  = (seq_t)1;
            for (idx_t j = 1; j <= exp; ++j)
            {
                term *= (seq_t)p;
                sum  += term;
            }
            if ((exp & 1) == 0)         /* even exponent → subtract p^{e/2} */
            {
                seq_t pow_half = (seq_t)1;
                for (idx_t j = 0; j < exp / 2; ++j)
                    pow_half *= (seq_t)p;
                sum -= pow_half;
            }
            total *= sum;
        }
    }

    if (tmp > 1)                     /* remaining prime factor with exp = 1 */
    {
        seq_t p = (seq_t)tmp;
        total *= ((seq_t)1 + p);      /* no subtraction for odd exponent */
    }

    return total;
}

/* --- cache of found bi‑unitary abundant numbers --------------------------- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t  next_k     = 1;        /* next integer to test */

/* append a new term to the cache */
static void append_cache(seq_t value)
{
    if (cache_len == cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = value;
}

/* --- public sequence routine --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n < 1, return 0 */

    init_primes();

    while (cache_len < (size_t)n)
    {
        seq_t bs = bsigma_value(next_k);
        if (bs > (seq_t)(2 * (seq_t)next_k))
            append_cache((seq_t)next_k);
        ++next_k;
    }
    return cache[(size_t)(n - 1)];
}
