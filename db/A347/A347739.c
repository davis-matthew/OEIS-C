/*
 * A347739  Number of compositions (ordered partitions) of n into at most 2
 *          prime parts.
 * https://oeis.org/A347739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (is_prime(n) ? 1 : 0) + number of ordered prime pairs
 *            (p,q) with p+q = n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple sieve of Eratosthenes on demand and caches the result. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Global cached data for primes */
static uint64_t *primes = NULL;        /* array of prime numbers */
static size_t prime_count = 0;         /* number of stored primes */
static bool   *isprime = NULL;         /* boolean table up to current_limit */
static idx_t   current_limit = 0;      /* highest n for which sieve computed */

/* Recompute sieve and prime list up to at least n. */
static void ensure_prime_limit(idx_t n)
{
    if (n <= current_limit)
        return;

    /* Allocate and initialize boolean array up to n */
    bool *new_isprime = (bool *)malloc((n + 1) * sizeof(bool));
    if (!new_isprime) abort();
    for (idx_t i = 0; i <= n; ++i)
        new_isprime[i] = true;
    if (n >= 0) new_isprime[0] = false;
    if (n >= 1) new_isprime[1] = false;

    /* Sieve */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (new_isprime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                new_isprime[j] = false;
        }
    }

    /* Count primes and build prime array */
    size_t new_count = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (new_isprime[i]) ++new_count;

    uint64_t *new_primes = NULL;
    if (new_count > 0) {
        new_primes = (uint64_t *)malloc(new_count * sizeof(uint64_t));
        if (!new_primes) abort();
        size_t idx = 0;
        for (idx_t i = 2; i <= n; ++i)
            if (new_isprime[i])
                new_primes[idx++] = i;
    }

    /* Replace old data */
    free(isprime);
    free(primes);
    isprime = new_isprime;
    primes = new_primes;
    prime_count = new_count;
    current_limit = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;  /* empty composition */

    ensure_prime_limit(n);

    seq_t count = 0;
    if (isprime[n])
        ++count;           /* composition with exactly one prime part */

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p > n)
            break;
        idx_t q = n - p;
        if (q < 2)
            continue;
        if (isprime[q])
            ++count;       /* composition (p, q) */
    }
    return count;
}
