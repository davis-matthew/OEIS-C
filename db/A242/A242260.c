/*
 * A242260  Primes p such that p^2-2 is semiprime.
 * https://oeis.org/A242260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 2512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A242260: primes p such that p^2-2 is a semiprime (product of two primes).
   The function finds the n‑th such prime by iterating over all primes.
   It memoises already found primes and continues from where it left off,
   so repeated calls in arbitrary order are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const idx_t SIEVE_LIMIT = 2000000;   /* ample for n up to ~10 000 */
static uint8_t *is_prime = NULL;            /* sieve storage */
static uint32_t *primes = NULL;              /* list of primes up to SIEVE_LIMIT */
static size_t primes_count = 0;
static size_t primes_processed = 0;          /* index of next prime to test */

static seq_t *qualifiers = NULL;            /* array of qualifying primes */
static size_t qual_count = 0;
static size_t qual_capacity = 0;

/* Create a simple sieve and fill the primes array. */
static void init_sieve(void)
{
    if (is_prime != NULL)
        return;    /* already initialized */

    is_prime = (uint8_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint8_t));
    if (!is_prime)
        return;    /* allocation failed – the build system will handle it */

    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; (idx_t)i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;

    /* Count primes first to allocate correctly. */
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            ++primes_count;

    primes = (uint32_t *)malloc(primes_count * sizeof(uint32_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            primes[idx++] = (uint32_t)i;
}

/* Ensure the qualifiers array has space for at least n elements. */
static void ensure_qual_capacity(idx_t n)
{
    if (qual_capacity >= (size_t)n)
        return;
    size_t new_cap = qual_capacity ? qual_capacity * 2 : 64;
    if (new_cap < (size_t)n)
        new_cap = n;
    seq_t *tmp = (seq_t *)realloc(qualifiers, new_cap * sizeof(seq_t));
    if (tmp)
    {
        qualifiers = tmp;
        qual_capacity = new_cap;
    }
}

/* Test whether a 64‑bit number is a semiprime (exactly two prime factors). */
static bool is_semiprime(uint64_t m)
{
    if (m < 6)
        return false;   /* smallest semiprime is 2*3 = 6 */

    uint64_t spf = 0;   /* smallest prime factor */
    for (size_t i = 0; i < primes_count; ++i)
    {
        uint32_t p = primes[i];
        uint64_t p2 = (uint64_t)p * p;
        if (p2 > m)
            break;
        if (m % p == 0)
        {
            spf = p;
            break;
        }
    }
    if (spf == 0)   /* m itself is prime */
        return false;

    uint64_t t = m / spf;
    if (t <= 1)
        return false;

    /* Check that t is prime. */
    bool t_is_prime = true;
    for (size_t i = 0; i < primes_count; ++i)
    {
        uint32_t p = primes[i];
        uint64_t p2 = (uint64_t)p * p;
        if (p2 > t)
            break;
        if (t % p == 0)
        {
            t_is_prime = false;
            break;
        }
    }
    return t_is_prime;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1, but harmless */

    init_sieve();
    ensure_qual_capacity(n);

    /* Keep extending the qualifiers list until we have at least n terms. */
    while (qual_count < n)
    {
        if (primes_processed >= primes_count)
            return (seq_t)0;   /* should never happen with a large enough SIEVE_LIMIT */

        uint32_t p = primes[primes_processed++];
        uint64_t m = (uint64_t)p * (uint64_t)p - 2ull;
        if (is_semiprime(m))
            qualifiers[qual_count++] = (seq_t)p;
    }

    return qualifiers[n - 1];
}
