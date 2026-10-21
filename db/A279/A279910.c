/*
 * A279910  a(n) = Sum_{k=1..n} prime(k+1)*floor(n/prime(k+1)).
 * https://oeis.org/A279910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p odd prime ≤ n} p * floor(n / p)
 * Range:     u8 n <= 32, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A279910. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static state for a prime cache. */
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t   prime_count  = 0;       /* number of stored primes */
static idx_t    prime_limit  = 0;       /* largest limit that was sieved */

static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit || limit < 2)
        return;  /* already sufficient or trivially empty */

    /* Allocate sieve array.  Cast to avoid C++ void* conversion error. */
    size_t sz = (size_t)limit + 1;
    bool *sieve = (bool*)calloc(sz, sizeof(bool));
    if (!sieve)
        return;    /* allocation failure – leave cache unchanged */

    /* Eratosthenes sieve. */
    for (idx_t p = 2; (idx_t)(p * p) <= limit; ++p)
    {
        if (!sieve[p])
        {
            for (idx_t q = p * p; q <= limit; q += p)
                sieve[q] = true;
        }
    }

    /* Count primes and build list. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            ++cnt;

    uint64_t *new_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_list) {
        free(sieve);
        return;    /* allocation failure – keep old cache */
    }

    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            new_list[k++] = (uint64_t)i;
        }
    }

    free(sieve);
    free(prime_list);

    prime_list   = new_list;
    prime_count  = cnt;
    prime_limit  = limit;
}

/* Main function required by the build system */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                /* no odd primes ≤ n */

    ensure_primes(n);

    seq_t total = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p == 2)           /* skip the prime 2 (even) */
            continue;
        idx_t q = n / p;      /* floor division */
        seq_t term = (seq_t)p * (seq_t)q;
        total += term;
    }
    return total;
}
