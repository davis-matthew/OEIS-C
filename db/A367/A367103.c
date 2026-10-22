/*
 * A367103  Composite numbers c such that pi(c) is the product of pi applied
 *          to the distinct prime factors of c.
 * https://oeis.org/A367103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   OEIS A367103 – composite numbers c with π(c)=∏π(p) for distinct
 *            prime factors p|c
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* ---------- global state ---------- */
static uint64_t *prime_list = NULL;    /* list of primes */
static size_t prime_cnt = 0;           /* number of primes stored */
static size_t prime_cap = 0;           /* capacity of prime_list */

static uint8_t *is_prime_arr = NULL;   /* sieve array */
static idx_t sieve_limit = 0;          /* current sieve size */

static uint64_t *term_list = NULL;     /* found terms */
static size_t term_cnt = 0;
static size_t term_cap = 0;

static uint64_t last_checked = 2;     /* last candidate processed */

/* ---------- helper functions ---------- */

/* add a new prime to the list, resizing if necessary */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        prime_cap = prime_cap ? prime_cap << 1 : 16;
        prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    }
    prime_list[prime_cnt++] = p;
}

/* generate a sieve of Eratosthenes up to 'limit' */
static void init_sieve(idx_t limit)
{
    /* free previous allocations if any */
    free(is_prime_arr);
    free(prime_list);
    prime_list = NULL;
    prime_cnt = 0;
    prime_cap = 0;
    is_prime_arr = (uint8_t *)malloc((size_t)(limit + 1));
    memset(is_prime_arr, 1, (size_t)(limit + 1));
    is_prime_arr[0] = 0;
    is_prime_arr[1] = 0;

    idx_t i;
    for (i = 2; (idx_t)i * (idx_t)i <= limit; ++i) {
        if (is_prime_arr[i]) {
            for (idx_t j = (idx_t)i * (idx_t)i; j <= limit; j += i)
                is_prime_arr[j] = 0;
        }
    }
    for (i = 2; i <= limit; ++i)
        if (is_prime_arr[i]) add_prime(i);

    sieve_limit = limit;
}

/* ensure sieve covers at least 'limit' */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;
    idx_t new_limit = limit * 2;
    init_sieve(new_limit);
}

/* binary search to compute π(x) over stored primes */
static idx_t prime_pi(uint64_t x)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (idx_t)lo;
}

/* ---------- sequence computation ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1, but safe */

    static bool initialized = false;
    if (!initialized) {
        /* 20 million gives ~1.27 million primes, enough for n up to at least 10000 */
        init_sieve(20000000U);
        initialized = true;
    }

    while (term_cnt < (size_t)n) {
        ++last_checked;
        /* skip primes */
        if (last_checked <= sieve_limit && is_prime_arr[last_checked])
            continue;
        /* ensure we can compute π(c) for this number */
        ensure_sieve(last_checked);

        /* factor the composite number */
        uint64_t pr[64];
        uint64_t ex[64];
        size_t fac_cnt = 0;
        factor_u64((uint64_t)last_checked, pr, ex, &fac_cnt);

        /* compute product of π(p) over distinct prime factors */
        seq_t prod = (seq_t)1;
        for (size_t i = 0; i < fac_cnt; ++i) {
            seq_t idx = (seq_t)prime_pi(pr[i]);
            prod *= idx;
        }

        /* compute π(c) */
        seq_t pi_val = (seq_t)prime_pi((uint64_t)last_checked);

        /* property check */
        if (prod == pi_val) {
            if (term_cnt == term_cap) {
                term_cap = term_cap ? term_cap << 1 : 16;
                term_list = (uint64_t *)realloc(term_list, term_cap * sizeof(uint64_t));
            }
            term_list[term_cnt++] = (uint64_t)last_checked;
        }
    }

    return (seq_t)term_list[n - 1];
}
