/*
 * A266953  Least prime p such that p-2 and 6n-p and either 6n+2-p or 6n+4-p
 *          is also prime, or 0 if no such prime exists.
 * https://oeis.org/A266953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that * p, p-2, 6n-p are prime, and *
 *            either 6n+2-p or 6n+4-p is prime; return 0 if no such p exists
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static sieve data */
static unsigned char *sieve = NULL;   /* 0=non‑prime, 1=prime */
static idx_t sieve_limit = 0;        /* largest number sieved so far */
static uint64_t *prime_list = NULL;  /* list of primes <= sieve_limit */
static size_t prime_count = 0;       /* number of primes in prime_list */

/* Build a sieve up to at least 'limit' */
static void _ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* Allocate new sieve array of size limit+1 */
    unsigned char *new_sieve = (unsigned char *)malloc((size_t)(limit + 1));
    if (!new_sieve) abort();  /* allocation failure */
    for (size_t i = 0; i <= (size_t)limit; ++i) new_sieve[i] = 1;
    new_sieve[0] = new_sieve[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p)
    {
        if (!new_sieve[(size_t)p]) continue;
        for (idx_t m = p * p; m <= limit; m += p)
            new_sieve[(size_t)m] = 0;
    }

    /* Count primes to determine array size */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (new_sieve[(size_t)i]) ++cnt;

    uint64_t *new_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_list) abort();
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (new_sieve[(size_t)i]) new_list[idx++] = (uint64_t)i;

    /* Replace old data */
    free(sieve);
    free(prime_list);
    sieve = new_sieve;
    prime_list = new_list;
    prime_count = cnt;
    sieve_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* a(0) = 0 explicitly */
    idx_t limit = 6 * n;         /* need primes up to 6n */
    _ensure_sieve(limit);        /* make sure primes up to 6n are available */

    for (size_t k = 0; k < prime_count; ++k)
    {
        uint64_t p = prime_list[k];
        if (p > limit) break;        /* p cannot exceed 6n */

        /* p-2 must be prime */
        if (p < 5) continue;         /* smallest twin prime is 5 (5-2=3) */
        if (!sieve[(size_t)(p - 2)])
            continue;

        /* 6n - p must be prime */
        idx_t q1 = limit - p;
        if (q1 < 2 || !sieve[(size_t)q1])
            continue;

        /* 6n+2-p or 6n+4-p must be prime */
        idx_t q2 = limit + 2 - p;
        idx_t q3 = limit + 4 - p;
        if ((!sieve[(size_t)q2]) && (!sieve[(size_t)q3]))
            continue;

        /* All conditions satisfied: return p */
        return (seq_t)p;
    }
    return 0;   /* no suitable prime found */
}
