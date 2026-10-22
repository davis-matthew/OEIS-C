/*
 * A355196  Sum of the largest parts of the partitions of n into exactly 3
 *          prime parts.
 * https://oeis.org/A355196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the largest prime in each unordered * partition of
 *            n into exactly three primes. * That is, a(n) = Σ r, over all
 *            primes p ≤ q ≤ r with * p + q + r = n
 * Range:     u8 n <= 40, u16 n <= 382, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static cache for the prime sieve and prime list. */
static uint8_t  *sieve        = NULL;   /* 0 = prime, 1 = composite      */
static uint64_t *prime_list   = NULL;   /* list of primes up to current limit */
static size_t   prime_count   = 0;      /* number of primes stored         */
static idx_t    sieve_limit   = 0;      /* maximum n that has been sieved   */

/* Build (or extend) a sieve of Eratosthenes up to 'limit'. */
static void
ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                          /* already good enough */

    /* Free old structures if they exist. */
    if (sieve)    free(sieve);
    if (prime_list) free(prime_list);

    /* Allocate sieve array (0 = potentially prime). */
    sieve = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!sieve) exit(1);                 /* allocation failed */
    for (idx_t i = 0; i <= limit; ++i)
        sieve[i] = 0;                     /* assume prime */

    /* Eratosthenes sieve. */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!sieve[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                sieve[q] = 1;             /* composite */
        }
    }

    /* Count primes and build list. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) exit(1);
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            prime_list[idx++] = i;
        }
    }

    prime_count = cnt;
    sieve_limit = limit;
}

/* External function required by the framework. */
seq_t
a(idx_t n)
{
    /* The smallest sum of three primes is 2+2+2 = 6. */
    if (n < 6)
        return 0;

    ensure_sieve(n);

    seq_t sum = 0;

    /* Enumerate unordered triples (p ≤ q ≤ r). */
    for (size_t ip = 0; ip < prime_count; ++ip) {
        idx_t p = prime_list[ip];
        if (p > n/3)               /* p too large, since p ≤ q ≤ r */
            break;

        for (size_t iq = ip; iq < prime_count; ++iq) {
            idx_t q = prime_list[iq];
            idx_t r = n - p - q;
            if (r < q)             /* r would be smaller than q, no more valid q */
                break;
            if (r > n)             /* should not happen, but guard anyway */
                continue;
            if (!sieve[r])         /* r is prime */
                sum += (seq_t)r;   /* add largest part */
        }
    }

    return sum;
}
