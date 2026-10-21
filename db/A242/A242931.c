/*
 * A242931  Largest number k such that k*n/(k+n) is prime or 0 if no such k
 *          exists.
 * https://oeis.org/A242931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 257, u32 n <= 9463, u64 n <= 9463,
 *            u128 n <= 9463, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A242931:  largest k such that k*n/(k+n) is prime; returns 0 if none. */
#include <stdlib.h>
#include <stdint.h>

/* Static storage for primes and sieve */
static idx_t  prime_limit = 0;          /* maximum number already sieved */
static size_t prime_count = 0;          /* number of primes stored      */
static uint64_t *prime_list = NULL;     /* array of primes              */
static unsigned char *sieve = NULL;     /* boolean sieve array          */

/* Build a prime sieve up to 'limit' (inclusive) and store primes. */
static void build_prime_sieve(idx_t limit)
{
    if (limit <= prime_limit) return;   /* already computed */

    /* free old allocations */
    free(sieve);
    free(prime_list);

    /* allocate sieve array, mark all as potential prime */
    sieve = (unsigned char*)malloc((limit + 1) * sizeof(unsigned char));
    if (!sieve) return;          /* allocation failure – unlikely for given limits */

    for (idx_t i = 0; i <= limit; ++i) sieve[i] = 1;
    if (limit >= 0) sieve[0] = 0;
    if (limit >= 1) sieve[1] = 0;

    /* simple Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i) sieve[j] = 0;
        }
    }

    /* count primes and store them */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (sieve[i]) ++cnt;
    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) {
        prime_limit = 0;
        prime_count = 0;
        return;
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) if (sieve[i]) {
        prime_list[pos++] = (uint64_t)i;
    }
    prime_limit = limit;
    prime_count = cnt;
}

seq_t a(idx_t n)
{
    /* n <= 2 cannot have a prime p < n, so result is 0 */
    if (n <= 2) return (seq_t)0;

    /* prepare primes up to n-1 (largest prime < n) */
    build_prime_sieve(n - 1);

    /* iterate primes from largest to smallest */
    for (size_t idx = prime_count; idx > 0; ) {
        --idx;
        uint64_t p = prime_list[idx];          /* current prime < n */
        if (p >= (uint64_t)n) continue;        /* safety check */

        idx_t denom = n - (idx_t)p;            /* n - p  (positive) */
        seq_t num   = (seq_t)p * (seq_t)n;      /* p * n */

        if (num % (seq_t)denom == 0) {
            /* integer k found */
            return num / (seq_t)denom;
        }
    }

    /* no prime gave an integer k */
    return (seq_t)0;
}
