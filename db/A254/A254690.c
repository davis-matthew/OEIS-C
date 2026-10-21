/*
 * A254690  Number of decompositions of 2n into a sum of two primes p1 < p2
 *          such that p2-p1 is between a pair of sexy primes.
 * https://oeis.org/A254690
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

/* a(n) = number of decompositions of 2n into two primes p1<p2 such that 
   p2‑p1 is strictly between a pair of sexy primes (p, p+6). 
   Computed by extending a sieve of Eratosthenes and counting suitable
   prime pairs for the requested n. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- static storage for the sieve and auxiliary tables ---- */
static uint8_t *is_prime = NULL;   /* 1 if prime, 0 otherwise, indexed by number */
static size_t  prime_limit = 0;    /* current maximum number sieved */

static uint32_t *primes = NULL;    /* list of primes up to prime_limit */
static size_t   prime_cnt = 0;     /* number of primes in the list */

static bool *good_diff = NULL;     /* good_diff[d] == true if d lies strictly
                                     between a sexy‑prime pair. */
static size_t good_limit = 0;      /* size of good_diff (== prime_limit) */

/* ---- rebuild the prime list from is_prime -------------------- */
static void rebuild_prime_list(void)
{
    size_t cnt = 0;
    for (size_t i = 2; i <= prime_limit; ++i)
        if (is_prime[i]) ++cnt;

    uint32_t *new_primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_primes) exit(EXIT_FAILURE);

    size_t j = 0;
    for (size_t i = 2; i <= prime_limit; ++i)
        if (is_prime[i]) new_primes[j++] = (uint32_t)i;

    free(primes);
    primes = new_primes;
    prime_cnt = cnt;
}

/* ---- rebuild the good_diff array ---------------------------- */
static void rebuild_good_diff(void)
{
    bool *new_good = (bool *)calloc(prime_limit + 1, sizeof(bool));
    if (!new_good) exit(EXIT_FAILURE);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if (p + 6 > prime_limit) break;          /* no further sexy pairs */
        if (is_prime[p + 6]) {
            size_t start = p + 1;
            size_t end   = p + 5;                 /* exclusive upper bound p+6 */
            if (end > prime_limit) end = prime_limit;
            for (size_t d = start; d <= end; ++d)
                new_good[d] = true;
        }
    }

    free(good_diff);
    good_diff = new_good;
    good_limit = prime_limit;
}

/* ---- extend the sieve to at least new_limit ----------------- */
static void expand_sieve(size_t new_limit)
{
    if (new_limit <= prime_limit) return;        /* already sufficient */

    uint8_t *new_is_prime = (uint8_t *)calloc(new_limit + 1, sizeof(uint8_t));
    if (!new_is_prime) exit(EXIT_FAILURE);

    /* mark all as prime initially (except 0 and 1) */
    for (size_t i = 2; i <= new_limit; ++i)
        new_is_prime[i] = 1;

    /* perform Eratosthenes */
    for (size_t i = 2; i * i <= new_limit; ++i)
        if (new_is_prime[i])
            for (size_t j = i * i; j <= new_limit; j += i)
                new_is_prime[j] = 0;

    free(is_prime);
    is_prime = new_is_prime;
    prime_limit = new_limit;

    rebuild_prime_list();
    rebuild_good_diff();
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* below offset – arbitrary */

    size_t target = (size_t)n * 2;              /* we need primes up to 2n */

    /* ensure tables contain all numbers up to target */
    expand_sieve(target);

    seq_t count = 0;

    /* iterate over all prime p1 < n */
    for (size_t i = 0; i < prime_cnt && primes[i] < (size_t)n; ++i) {
        size_t p1 = primes[i];
        size_t p2 = target - p1;
        if (p2 <= target && p2 > p1 && is_prime[p2]) {
            size_t diff = p2 - p1;
            if (diff <= good_limit && good_diff[diff])
                ++count;
        }
    }
    return count;
}
