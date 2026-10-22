/*
 * A382766  Odd primes p such that p + 4, p + 6 and p + 8 are composite.
 * https://oeis.org/A382766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 3421, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function enumerates odd primes p such that p + 4,
 * p + 6 and p + 8 are composite.  It returns the n‑th such prime
 * (1‑based indexing; a(1) = 113).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/is_prime.h"

/* ----------  static cache for the sequence  ---------- */
static seq_t *cache      = NULL;   /* terms a(1), a(2), ...          */
static size_t cache_len  = 0;      /* number of terms cached        */
static size_t cache_cap  = 0;      /* capacity of cache array       */

/* grow cache to at least size 'need' (in elements) */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ----------  static sieve for primes  ---------- */
static uint64_t *prime_list   = NULL;   /* stored primes */
static size_t   prime_cnt    = 0;       /* number of primes stored */
static size_t   prime_cap    = 0;       /* capacity of prime_list */

/* grow prime list array to at least size 'need' (in elements) */
static void grow_prime_list(size_t need)
{
    if (need <= prime_cap)
        return;
    size_t new_cap = (prime_cap == 0) ? 64 : prime_cap;
    while (new_cap < need)
        new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(*prime_list));
    if (!tmp)
        abort();
    prime_list = tmp;
    prime_cap = new_cap;
}

/* init static structures on first call */
static void init_static(void)
{
    if (prime_list == NULL) {
        grow_prime_list(64);
        prime_list[prime_cnt++] = 2ULL;   /* include 2 for completeness */
    }
}

/* test whether a given odd number n (>=3) is prime using prime_list */
static bool is_prime_trial(uint64_t n)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n / p)   /* avoid overflow of p*p */
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* under the offset 1 */

    init_static();

    /* return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* start searching from the next unexplored odd integer */
    static uint64_t last_candidate = 3ULL;
    while (cache_len < (size_t)n) {
        /* find next prime */
        uint64_t cand = last_candidate;
        last_candidate += 2ULL;    /* candidates are odd */

        if (cand < 3ULL)           /* never happens */
            continue;

        /* trial division test; extend prime list if needed */
        grow_prime_list(prime_cnt + 1);
        if (!is_prime_trial(cand)) {
            continue;              /* skip composite candidate */
        }
        /* store the new prime */
        prime_list[prime_cnt++] = cand;

        /* check whether cand+4, cand+6 and cand+8 are all composite */
        uint64_t p4 = cand + 4ULL;
        uint64_t p6 = cand + 6ULL;
        uint64_t p8 = cand + 8ULL;

        /* use the provided is_prime function for correct handling
           of large values (e.g. when seq_t is big integer) */
        if (!is_prime(p4) && !is_prime(p6) && !is_prime(p8)) {
            /* cache the term */
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
