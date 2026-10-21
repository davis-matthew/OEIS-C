/*
 * A254688  Number of decompositions of 2n into sums of two primes p1 < p2
 *          such that p2-p1+1 is also a prime.
 * https://oeis.org/A254688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A254688 via brute‑force enumeration of prime pairs. */
/* The definition: a(n) = the number of representations 2n = p1+p2
   with p1 < p2 both prime and (p2−p1+1) also prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ----  Prime sieve  ---- */

static bool *prime_sieve = NULL;     /* sieve array: true if index is prime */
static uint64_t *prime_list = NULL;  /* array of primes */
static size_t prime_count = 0;       /* number of primes */
static idx_t sieve_limit = 0;        /* current maximum sieved value */

/* Simple trial division primality test for small numbers (used only for diff ≥ limit, if any). */
static bool is_prime_small(unsigned int x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (unsigned int i = 3; (uint64_t)i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

/* Extend the sieve to cover all numbers up to `max` inclusively. */
static void extend_sieve(idx_t max)
{
    if (max <= sieve_limit) return;               /* already covered */

    /* free previous sieve and prime list */
    free(prime_sieve);
    free(prime_list);

    /* allocate new sieve */
    prime_sieve = (bool*)malloc((size_t)(max + 1) * sizeof(bool));
    if (!prime_sieve) exit(1);
    for (idx_t i = 0; i <= max; ++i) prime_sieve[i] = true;
    if (max >= 0) prime_sieve[0] = prime_sieve[1] = false;

    /* Eratosthenes */
    for (idx_t p = 2; (uint64_t)p * p <= max; ++p)
        if (prime_sieve[p])
            for (idx_t q = p * p; q <= max; q += p)
                prime_sieve[q] = false;

    /* count primes */
    prime_count = 0;
    for (idx_t i = 2; i <= max; ++i)
        if (prime_sieve[i]) ++prime_count;

    /* store primes in an array */
    prime_list = (uint64_t*)malloc(prime_count * sizeof(uint64_t));
    if (!prime_list) exit(1);
    size_t k = 0;
    for (idx_t i = 2; i <= max; ++i)
        if (prime_sieve[i]) prime_list[k++] = (uint64_t)i;

    sieve_limit = max;
}

/* ----  Sequence a(n)  ---- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                         /* undefined for n < 1 */

    idx_t two_n = n * 2;                         /* 2n (fits in idx_t for the required ranges) */
    extend_sieve(two_n);

    seq_t count = 0;                             /* result */

    /* Enumerate prime pairs p1 < p2 with p1 + p2 = 2n */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p1 = prime_list[i];
        if (p1 >= two_n) break;                 /* no further useful pairs */

        uint64_t p2 = two_n - p1;
        if (p2 <= p1) continue;                 /* enforce p1 < p2 */

        if (p2 > sieve_limit)                  /* should not happen for n ≤ 10000 */
            continue;
        if (!prime_sieve[p2]) continue;         /* p2 not prime */

        uint64_t diff = p2 - p1 + 1;
        bool diff_prime;
        if (diff <= sieve_limit)
            diff_prime = prime_sieve[diff];
        else
            diff_prime = is_prime_small((unsigned int)diff);

        if (diff_prime) ++count;                /* valid decomposition */
    }

    return count;
}
