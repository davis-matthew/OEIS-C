/*
 * A373656  a(n) is the n-th prime after the n-th semiprime.
 * https://oeis.org/A373656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime after nth semiprime (computed by sieve)
 * Range:     u8 n <= 30, u16 n <= 4512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"

/* Cached prime list and spf array */
static uint64_t *prime_arr = NULL;     /* primes ascending */
static size_t   prime_cnt = 0;        /* number of primes stored */
static idx_t    sieve_limit = 0;      /* largest number currently sieved */
static idx_t   *spf_arr = NULL;       /* smallest prime factor for each number */

/* Ensure the sieve covers at least numbers up to 'limit'. */
static void
ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;   /* already adequate */

    /* free previous data */
    if (prime_arr) { free(prime_arr); prime_arr = NULL; }
    if (spf_arr)  { free(spf_arr);  spf_arr  = NULL; }

    if (limit < 16) limit = 16;          /* minimal sensible limit */

    build_primes_upto(limit, &prime_arr, &prime_cnt);
    build_spf(limit, &spf_arr);
    sieve_limit = limit;
}

/* Test whether x is a semiprime (product of two primes). */
static bool
is_semiprime(idx_t x)
{
    if (x < 4) return false;
    idx_t p = spf_arr[x];
    idx_t m = x / p;
    return (m > 1) && (spf_arr[m] == m);   /* m must be prime */
}

/* Binary search: number of primes ≤ x. */
static idx_t
prime_count_up_to(idx_t x)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_arr[mid] <= x) lo = mid + 1;
        else                     hi = mid;
    }
    return (idx_t)lo;
}

/* Cached semiprime list. */
static idx_t *semiprime_cache = NULL;
static size_t semiprime_cap = 0;
static size_t semiprime_cnt = 0;
static idx_t next_candidate = 4;

/* Ensure that semiprime_cache contains at least 'desired' semiprimes. */
static void
ensure_semiprimes(idx_t desired)
{
    while (semiprime_cnt < desired) {
        /* Expand the sieve if needed. */
        if (next_candidate > sieve_limit) {
            idx_t new_limit = sieve_limit * 2;
            if (new_limit <= next_candidate) new_limit = next_candidate;
            if (new_limit < 16) new_limit = 16;
            ensure_sieve(new_limit);
        }

        if (is_semiprime(next_candidate)) {
            /* Grow cache if necessary. */
            if (semiprime_cnt >= semiprime_cap) {
                size_t new_cap = semiprime_cap ? semiprime_cap * 2 : 64;
                idx_t *tmp = (idx_t *)realloc(semiprime_cache, new_cap * sizeof(idx_t));
                if (!tmp) abort();
                semiprime_cache = tmp;
                semiprime_cap   = new_cap;
            }
            semiprime_cache[semiprime_cnt++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* offset starts at 1 */

    /* Minimum initial sieve. */
    ensure_sieve(16);

    /* Get the n‑th semiprime. */
    ensure_semiprimes(n);
    idx_t semval = semiprime_cache[n - 1];

    /* Count primes ≤ that semiprime. */
    idx_t pi_s = prime_count_up_to(semval);

    /* Desired prime index (1‑based). */
    idx_t target = pi_s + n;

    /* Grow prime sieve to contain the target prime. */
    while (target > prime_cnt) {
        idx_t new_limit = sieve_limit * 2;
        if (new_limit < sieve_limit + 1) new_limit = sieve_limit + 1;
        if (new_limit < target) new_limit = target * 2;
        ensure_sieve(new_limit);
    }

    /* Return the target prime. */
    return (seq_t)prime_arr[target - 1];
}
