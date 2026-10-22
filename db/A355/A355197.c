/*
 * A355197  Sum of the middle parts of the partitions of n into exactly 3
 *          prime parts.
 * https://oeis.org/A355197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A355197
   Formula:  a(n) = ∑_{j=1..⌊n/3⌋} ∑_{i=j..⌊(n−j)/2⌋} c(i)*c(j)*c(n−i−j)*i
   where c(k) is 1 if k is prime, 0 otherwise.
   This sums the middle part of every unordered partition of n into exactly
   three prime parts.  The function builds a prime sieve up to the
   largest n seen and uses it for all calls. */
#include <stdlib.h>
#include <stdint.h>

static size_t sieve_limit = 0;          /* current sieve upper bound       */
static unsigned char *is_prime  = NULL; /* primality flags, indexed by value*/
static uint64_t *primes = NULL;       /* array of primes in ascending order*/
static size_t prime_count = 0;        /* number of primes stored          */

/* Build or extend the sieve up to 'limit' (inclusive).  */
static void ensure_sieve(idx_t limit)
{
    if ((idx_t)sieve_limit >= limit)
        return; /* already built */

    /* free previous arrays if any */
    free(is_prime);
    free(primes);

    size_t max = (size_t)limit;
    /* allocate primality flag array */
    is_prime = (unsigned char *)malloc((max + 1) * sizeof(unsigned char));
    if (!is_prime)
        return; /* allocation failure: subsequent queries will yield 0 */
    /* assume all numbers are prime initially */
    for (size_t i = 0; i <= max; ++i) is_prime[i] = 1;
    if (max >= 0) is_prime[0] = 0;
    if (max >= 1) is_prime[1] = 0;

    /* sieve of Eratosthenes */
    for (size_t i = 2; i * i <= max; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= max; j += i)
                is_prime[j] = 0;

    /* count primes to allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= max; ++i)
        if (is_prime[i]) ++cnt;

    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes)
        return; /* allocation failure */

    /* fill primes array */
    size_t idx = 0;
    for (size_t i = 2; i <= max; ++i)
        if (is_prime[i])
            primes[idx++] = (uint64_t)i;

    prime_count = cnt;
    sieve_limit = max;
}

/* Compute the desired sequence value for a given n.  */
seq_t a(idx_t n)
{
    /* The smallest sum of three primes is 2+2+2=6. */
    if (n < 6)
        return 0;

    /* Build sieve up to n if needed. */
    ensure_sieve(n);
    if (!is_prime)                /* allocation failure handling */
        return 0;

    seq_t result = 0;

    /* Find index of last prime <= n/3 to bound outer loop */
    size_t max_j_idx = 0;
    for (size_t i = 0; i < prime_count; ++i)
        if (primes[i] <= (idx_t)n / 3)
            ++max_j_idx;
    /* max_j_idx now points to count of primes <= n/3 */
    for (size_t j = 0; j < max_j_idx; ++j) {
        uint64_t pj = primes[j];                /* smallest prime (j) */

        /* Upper bound for middle prime i: floor((n - pj)/2) */
        uint64_t i_limit = ((idx_t)n - pj) / 2;

        /* Find index of last prime <= i_limit */
        size_t max_i_idx = j;
        for (size_t ii = j; ii < prime_count; ++ii) {
            if (primes[ii] <= i_limit)
                ++max_i_idx;
            else
                break;
        }

        for (size_t ii = j; ii < max_i_idx; ++ii) {
            uint64_t pi = primes[ii];               /* middle prime (i) */
            uint64_t pk = (idx_t)n - pj - pi;       /* largest prime (k) */

            /* pk >= pi is guaranteed by bounds, but check primality */
            if (pk <= (idx_t)n && pk >= pi && is_prime[pk])
                result += (seq_t)pi;
        }
    }

    return result;
}
