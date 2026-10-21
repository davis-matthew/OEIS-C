/*
 * A243485  Sum of all the products formed by multiplying the corresponding
 *          smaller and larger parts of the Goldbach partitions of n.
 * https://oeis.org/A243485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..⌊n/2⌋} i*(n-i) * [isPrime(i)] * [isPrime(n-i)]
 *            (sum of products of the two primes in all unordered Goldbach
 *            partitions of n)
 * Range:     u8 n <= 21, u16 n <= 179, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Builds an incremental sieve of Eratosthenes and caches the prime
   list and a prime‑mask. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Prime sieve state – private to this translation unit */
static uint64_t *prime_list      = NULL;   /* list of prime values      */
static size_t   prime_count      = 0;      /* number of primes stored   */
static idx_t    prime_limit      = 0;      /* largest n for which sieve
                                           is built                   */
static unsigned char *prime_mask = NULL;   /* primality bit‑mask, 1 = prime */

/* Integer square root (floor), implemented with binary search. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)   /* mid*mid > n */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Rebuild the sieve and prime list up to at least 'limit'. */
static void ensure_prime_sieve(idx_t limit)
{
    if (limit <= prime_limit)
        return;                       /* already up to date */

    /* Free previous structures */
    if (prime_list)  free(prime_list);
    if (prime_mask) free(prime_mask);

    /* Allocate mask and perform sieve */
    prime_mask = (unsigned char *)calloc((size_t)(limit + 1), sizeof(unsigned char));
    if (!prime_mask)
        return;                       /* allocation failed – caller will get 0 */

    prime_mask[0] = prime_mask[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        prime_mask[i] = 1;

    idx_t root = isqrt_u64(limit);
    for (idx_t p = 2; p <= root; ++p) {
        if (!prime_mask[p])
            continue;
        for (idx_t mult = p * p; mult <= limit; mult += p)
            prime_mask[mult] = 0;
    }

    /* Count primes to allocate list */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_mask[i])
            ++cnt;

    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) {
        free(prime_mask);
        prime_mask = NULL;
        return;
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_mask[i])
            prime_list[pos++] = (uint64_t)i;

    prime_count = cnt;
    prime_limit = limit;
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                 /* a(1)=a(2)=a(3)=0 by definition */

    ensure_prime_sieve(n);        /* build or extend sieve up to n */

    if (!prime_mask)
        return 0;                 /* sieve failed – harmless default */

    seq_t sum = 0;
    idx_t half = n / 2;

    /* Iterate over primes <= half */
    for (size_t i = 0; i < prime_count && prime_list[i] <= half; ++i) {
        uint64_t p = prime_list[i];
        uint64_t q = (uint64_t)(n - p);
        if (prime_mask[n - p])   /* q is prime */
            sum += (seq_t)p * (seq_t)q;
    }
    return sum;
}
